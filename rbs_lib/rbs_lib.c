#include "rbs_lib.h"

struct timespec time_reference;
struct log_event_data *log_event_buffers_ptrs[1600];
u_int32_t buff_indexes[1600];


void RBS_InitializeSequence(struct task_data *taskDATA, int sequenceID, pthread_t *thread, pthread_attr_t attr, void *(*func)())
{
	//Allocate memory for sequence data
	struct sequence_data *sequenceDATA = malloc(sizeof(struct sequence_data));
	
	sequenceDATA->task = taskDATA;
	sequenceDATA->sequence_id = sequenceID;
	sequenceDATA->current_job = taskDATA->last_added_job;
	
	sem_t *semaphore = taskDATA->sequences_guards + (sequenceID - 1);
	
	sequenceDATA->semaphore = semaphore;
	
	//Initialize semaphore
	sem_init(semaphore, 0, 0);
	
	//Initialize thread
    int result = pthread_create(thread, &attr, func, (void*) sequenceDATA);
 
}

void RBS_InitializeRBS()
{
    clock_gettime(CLOCK_REALTIME, &time_reference);
}

int RBS_InitializeTask(struct task_data *taskDATA)
{
    int result = 0;

    if((taskDATA->priority > 99) || (taskDATA->priority < 1))
    {
        //Priority out of range
        return 1;
    }

    //Allocate memory for previous job, this structure is needed only for purpose of correct functioning of the double linked list
    taskDATA->last_added_job = malloc(sizeof(struct job_token));
    taskDATA->last_added_job->secondary_sequences_guards = NULL;

    //SET SCHEDULER TO SCHED_FIFO and set priority of the task
    pthread_attr_init(&taskDATA->attr);
    result = pthread_attr_setschedpolicy(&taskDATA->attr, SCHED_FIFO);
    taskDATA->schedPARAM.sched_priority = taskDATA->priority;
    result = pthread_attr_setschedparam(&taskDATA->attr, &taskDATA->schedPARAM);
    result = pthread_attr_setinheritsched(&taskDATA->attr, PTHREAD_EXPLICIT_SCHED);

    //Initialize log buffer
	#ifdef LOG_DATA
        for(int buff_nr = 0; buff_nr <= taskDATA->number_of_sequences; buff_nr ++)
        {
            int index = (taskDATA->task_id - 1) * 40 + buff_nr;
            if(buff_nr == 0)
            {
                log_event_buffers_ptrs[index] = calloc(10, sizeof(struct log_event_data));
            }
            else
            {
                log_event_buffers_ptrs[index] = calloc(10000, sizeof(struct log_event_data));
            }
            buff_indexes[index] = 0;
        }
	#endif

    return 0;
}

struct log_event_data *log_event_start(int task, int sequence, int node, int job, int event)
{
    int index = (task - 1) * 40 + sequence;
    int local_buff_index = buff_indexes[index];

    struct log_event_data *ptr = log_event_buffers_ptrs[index] + local_buff_index;

    clock_gettime(CLOCK_REALTIME, &ptr->start_time);

    ptr->task = task;
    ptr->sequence = sequence;
    ptr->node = node;
    ptr->job = job;
    ptr->event = event;

    buff_indexes[index] = buff_indexes[index] + 1;

    return ptr;
}

void log_event_end(struct log_event_data *ptr)
{
    clock_gettime(CLOCK_REALTIME, &ptr->end_time);
}

void log_first_activation(int task, struct timespec activation)
{
    int index = (task - 1) * 40;
    int local_buff_index = buff_indexes[index];

    struct log_event_data *ptr = log_event_buffers_ptrs[index] + local_buff_index;

    ptr->task = task;
    ptr->sequence = 0;
    ptr->node = 0;
    ptr->job = 0;
    ptr->event = START_TIME_REL_FUNC;
    ptr->start_time.tv_sec = activation.tv_sec;
    ptr->start_time.tv_nsec = activation.tv_nsec;

    buff_indexes[index] = buff_indexes[index] + 1;
}

void set_cpu(int cpu_num)
{
   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(cpu_num, &cpuset);  
   sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}

void RBS_Wait(struct sequence_data *sequenceDATA)
{
	//Wait till a new job is released
	sem_wait(sequenceDATA->semaphore);
	
	//Update pointer
    sequenceDATA->current_job = sequenceDATA->current_job->next_job;

    if(sequenceDATA->sequence_id != 1)
    {
        //Wait till sequence can start
        sem_t * semaphore = sequenceDATA->current_job->secondary_sequences_guards + sequenceDATA->sequence_id - 2;
        sem_wait(semaphore);
    }   

}

void RBS_Release(struct task_data *taskDATA)
{

    //Increase the jobs counter
	taskDATA->job_counter = taskDATA->job_counter + 1;
	
	//allocate memory for new job token and set variables
    struct job_token *new_job = malloc(sizeof(struct job_token));
	new_job->job_id = taskDATA->job_counter;
    new_job->nodes_in_execution = 0;
    new_job->nodes_finished = 0;
    new_job->previous_job = taskDATA->last_added_job;

    //Allocate memory for the secondary guards of the job sequences and initialize them
    new_job->secondary_sequences_guards = calloc(taskDATA->number_of_sequences-1, sizeof(sem_t));

    //Initialize secondary sequence guards
    for(int i = 0; i < (taskDATA->number_of_sequences-1); i++)
    {
        sem_init((new_job->secondary_sequences_guards + i), 0, 0);
    }

    //initialize job data lock
    pthread_mutex_init(&new_job->job_lock, NULL);

    //update the pointer to the next job
    taskDATA->last_added_job->next_job = new_job;
    
    //post primary and secondary guard of the first sequence
    for(int i = 0; i < taskDATA->number_of_sequences; i++)
    {
        sem_post((taskDATA->sequences_guards + i));
    }
    //sem_post(new_job->secondary_sequences_guards);
    

    //update the pointer to the last job
    taskDATA->last_added_job = new_job;
}

void FinishJob(struct sequence_data *sequenceDATA)
{
    //If object is empty (first token is always empty) do nothing
    if(sequenceDATA->current_job->previous_job->secondary_sequences_guards == NULL)
    {
        return;
    }

    //Destroy mutex
    pthread_mutex_destroy(&sequenceDATA->current_job->previous_job->job_lock);

    //Destroy secondary semaphores
    for(int i = 0; i < sequenceDATA->task->number_of_sequences; i++)
    {
        sem_destroy((sequenceDATA->current_job->previous_job->secondary_sequences_guards + i));
    }
    
    //Free memory
    free(sequenceDATA->current_job->previous_job->secondary_sequences_guards);
    free(sequenceDATA->current_job->previous_job);
}

bool check_precedence_constraints(struct sequence_data *sequenceDATA, u_int32_t node_number)
{
    //source nodes doesn't have any precedence constraints
    if(node_number == 1)
    {
        return true;
    }
    
    uint64_t *ptr = (sequenceDATA->task->pre_cons_h + node_number - 1);
    if(*ptr == (*ptr & sequenceDATA->current_job->nodes_finished))
    {
        return true;
    }
    else
    {
        return false;
    }
    

}

void MarkNodeExecuted(struct sequence_data *sequenceDATA, int finished_node)
{

    //Mark task as finished by setting the bit in the job state variable
    uint64_t mask = 1;
    mask = mask << (finished_node - 1);
    sequenceDATA->current_job->nodes_finished = sequenceDATA->current_job->nodes_finished | mask;

}

int RBS_Execute(struct sequence_data *sequenceDATA, int node)
{
    
	//Lock job_token
    pthread_mutex_lock(&sequenceDATA->current_job->job_lock);
     
    //Check if the next node can be executed if not unlock job_token and return
    if((check_precedence_constraints(sequenceDATA, node)) == false)
    {
        pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
        return 1;
    }
    
    //Check if node is not already being executed if not unlock job_token and return
    if((check_if_node_in_execution(node, sequenceDATA->current_job)) == true)
    {
        pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
        return 2;
    } 
    
    //Change node state to being executed
    MarkNodeInExecution(sequenceDATA, node);
	
	//Unlock job_token
    pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
    
    //Log event
    #ifdef LOG_DATA
        struct log_event_data *log_ptr = log_event_start(sequenceDATA->task->task_id, sequenceDATA->sequence_id, node, sequenceDATA->current_job->job_id, NODE_EXECUTION);
    #endif
    
    //Execute Node function
    sequenceDATA->task->func[(node-1)]();
    
    //Log event
    #ifdef LOG_DATA
        log_event_end(log_ptr);
    #endif

	//Lock job_token
    pthread_mutex_lock(&sequenceDATA->current_job->job_lock);
    //Mark node as finished
    MarkNodeExecuted(sequenceDATA, node);
	//Unlock job_token
    pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);	
    
    //Signal head nodes of sequences with incoming precedence constraints from the finished node
    #ifdef AUTO_SIGNAL
    #ifdef LOG_DATA
            //log_ptr = log_event_start(sequenceDATA->task->task_id, sequenceDATA->sequence_id, node, sequenceDATA->current_job->job_id, SIGNALLING_EXECUTION);
        SignalSequenceAut(node, sequenceDATA);
           // log_event_end(log_ptr);
    #endif
    #endif

    
    return 0;
}

void SignalSequenceMan(struct sequence_data *sequenceDATA, int node_to_signal, sem_t *semaphore)
{
    bool signal_flag = check_precedence_constraints(sequenceDATA, node_to_signal);
    if(signal_flag == true)
    {
        sem_post(semaphore);
    }
}

void SignalSequenceAut(int finished_node, struct sequence_data *sequenceDATA)
{

    //Nodes that have incoming precedence constraints from the finished node
    uint64_t pre_cons = *(sequenceDATA->task->pre_cons_v + finished_node - 1);

    //If there are no nodes with incoming precedence constraints from the finshed node => nothing to signal
    if(pre_cons == 0)
    {
        return;
    }

    for(uint64_t i = 0; i < sequenceDATA->task->number_of_nodes; i ++)
    {
        uint64_t mask = 1;
        mask = mask << i;
                    
        if((mask & pre_cons) != 0)
        {
            //number of sequence of which the node is head of
            int sequence = *(sequenceDATA->task->sequence_heads + i);

            //if 0 than the node is not a head of any sequence
            if(sequence != 0)
            {
                if(check_precedence_constraints(sequenceDATA, (i+1)))
                {
                    //SIGNAL
                    sem_t *semaphore = sequenceDATA->current_job->secondary_sequences_guards + sequence - 2;	
                    sem_post(semaphore);
                    
                }
            }
        }
    }
}

bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer)
{
    uint64_t mask = 1;
    mask = mask << (node_number - 1);
    uint64_t local_execution_state = job_pointer->nodes_in_execution;
    
    local_execution_state = local_execution_state & mask;
    
    if(local_execution_state > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

void TerminateSequence(struct sequence_data *sequenceDATA, int node)
{
    #ifdef LOG_DATA
    //log_event_start(sequenceDATA->task->task_id, sequenceDATA->sequence_id, node, sequenceDATA->current_job->job_id, SEQUENCE_TERMINATED);
    #endif
}

void MarkNodeInExecution(struct sequence_data *sequenceDATA, int node)
{
    uint64_t mask = 1;
    mask = mask << (node - 1);
    sequenceDATA->current_job->nodes_in_execution = sequenceDATA->current_job->nodes_in_execution | mask;
}

void print_log_data_json(struct task_data **taskDATA_start, int num_of_tasks)
{
    FILE *fp;
    fp = fopen("log.json", "w");
    fprintf(fp, "{\"log\":[\n");

    for(int task = 0; task < num_of_tasks; task ++)
    {
        struct task_data *taskDATA = *(taskDATA_start + task);

        for(int i = 0 ; i <= taskDATA->number_of_sequences; i ++)
        {
            int index = (taskDATA->task_id - 1) * 40 + i;

            for(int x = 0; x < buff_indexes[index]; x++)
            {
                
                struct log_event_data *ptr = log_event_buffers_ptrs[index] + x;

                struct timespec start_time_loc;
                struct timespec end_time_loc;

                start_time_loc.tv_sec = ptr->start_time.tv_sec - time_reference.tv_sec;
                start_time_loc.tv_nsec = ptr->start_time.tv_nsec; 

                double start_time_stamp = (double)start_time_loc.tv_sec * 1000000;
                start_time_stamp = start_time_stamp + (start_time_loc.tv_nsec/1000);

                end_time_loc.tv_sec = ptr->end_time.tv_sec - time_reference.tv_sec;
                end_time_loc.tv_nsec = ptr->end_time.tv_nsec; 

                double time_stamp_end = (double)end_time_loc.tv_sec * 1000000;
                time_stamp_end = time_stamp_end + (end_time_loc.tv_nsec/1000);



                int task = ptr->task;
                int sequence = ptr->sequence;
                int node = ptr->node;
                int job = ptr->job;
                int event = ptr->event;

                if(task == (num_of_tasks) && x == (buff_indexes[index] - 1) && i == taskDATA->number_of_sequences)
                {
                    fprintf(fp, "{\"type\" : %d, \"task\" : %d, \"sequence\" : %d, \"node\" : %d, \"job\" : %d, \"start\" : %f, \"end\" : %f}\n",event, task, sequence, node, job, start_time_stamp, time_stamp_end);
                }
                else
                {
                    fprintf(fp, "{\"type\" : %d, \"task\" : %d, \"sequence\" : %d, \"node\" : %d, \"job\" : %d, \"start\" : %f, \"end\" : %f},\n",event, task, sequence, node, job, start_time_stamp, time_stamp_end);
                
                }

            
            }
        }
    }
    fprintf(fp, "]\n");
    fprintf(fp, "}\n");
    fclose(fp);

}
