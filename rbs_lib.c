#include "rbs_lib.h"


void InitializeSequence(struct task_data *taskDATA, int sequenceID, pthread_t *thread, pthread_attr_t attr, void *(*func)())
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

void initialize_rbs()
{
	#ifdef LOG_DATA
	openlog("RBS_IMPLEMENTATION_LOG", LOG_PID|LOG_CONS, LOG_USER);
	#endif
}

int InitializeTask(struct task_data *taskDATA)
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
    /////////////////////////////

    return 0;
}

void log_info(int task, int sequence, int node, int job, clock_t time_stamp, int event)
{
	if(event == NODE_EXECUTION_STARTED)
	{
		syslog(LOG_INFO, "NODE_EXECUTION_STARTED, task %d, sequence %d, node %d, job %d, at cycle: %f",task, sequence, node, job, (float)time_stamp);
	}
	else if(event == NODE_EXECUTION_FINISHED)
	{
		syslog(LOG_INFO, "NODE_EXECUTION_FINISHED, task %d, sequence %d, node %d, job %d, at cycle: %f",task, sequence, node, job, (float)time_stamp);
	}
	else if(event == NEW_JOB_RELEASED)
	{
		syslog(LOG_INFO, "NEW_JOB_RELEASED, task %d, job %d, at cycle: %f",task, job, (float)time_stamp);
	}
	else if(event == JOB_EXECUTION_FINISHED)
	{
		syslog(LOG_INFO, "JOB_EXECUTION_FINISHED, task %d, job %d, at cycle: %f",task, job, (float)time_stamp);
	}
    else if(event == SEQUENCE_TERMINATED)
    {
        syslog(LOG_INFO, "SEQUENCE_TERMINATED, task %d, sequence %d, after node %d, job %d, at cycle: %f",task, sequence, node, job, (float)time_stamp);
    }
	else
	{
		
	}
	
}

void set_cpu(int cpu_num)
{
   //int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(cpu_num, &cpuset);  
   sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}



void WaitNextJob(struct sequence_data *sequenceDATA)
{
	//Wait till a new job is released
	sem_wait(sequenceDATA->semaphore);
	
	//Update pointer
    sequenceDATA->current_job = sequenceDATA->current_job->next_job;


    //Wait till sequence can start
    sem_t * semaphore = sequenceDATA->current_job->secondary_sequences_guards + sequenceDATA->sequence_id - 1;
    sem_wait(semaphore);

}

void ReleaseNewJob(struct task_data *taskDATA)
{
	//Increase the jobs counter
	taskDATA->job_counter = taskDATA->job_counter + 1;
	
	//allocate memory for new job token
    struct job_token *new_job = malloc(sizeof(struct job_token));

	new_job->job_id = taskDATA->job_counter;
    new_job->job_execution_state = 0;
    new_job->job_state = 0;
    new_job->previous_job = taskDATA->last_added_job;

    //Allocate memory for the secondary guards of the job sequences and initialize them
    new_job->secondary_sequences_guards = calloc(taskDATA->number_of_sequences, sizeof(sem_t));

    //Initialize secondary sequence guards
    for(int i = 0; i < taskDATA->number_of_sequences; i++)
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
    sem_post(new_job->secondary_sequences_guards);
    
    #ifdef LOG_DATA
    clock_t time = clock();
    log_info(taskDATA->task_id, 0, new_job->job_id, new_job->job_id, time, NEW_JOB_RELEASED);
    #endif
    
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

bool check_precedence_constraints(struct sequence_data *sequenceDATA, u_int8_t node_number)
{
    //source nodes doesn't have any precedence constraints
    if(node_number == 1)
    {
        return true;
    }

    u_int8_t start_index = (node_number-2) * (sequenceDATA->task->number_of_nodes-1);
    u_int32_t mask = 0xFFFFFFFF;
    u_int32_t job_state_local = sequenceDATA->current_job->job_state & mask;
    bool *temp_bool_pointer = NULL;

    for(int x = 0; x < (sequenceDATA->task->number_of_nodes-1); x++)
    {
        u_int8_t temp_index = start_index + x;
        temp_bool_pointer = sequenceDATA->task->precedence_matrix + temp_index;

        if(*temp_bool_pointer == true)
        {
            mask = 1;
            mask = mask << x;
            if((mask & job_state_local) == 0)
            {
                return false;
            }
        }
    }

    return true;
}

void finish_node(struct sequence_data *sequenceDATA, int finished_node)
{
    int mask = 1;
    mask = mask << (finished_node - 1);

    //pthread_mutex_lock(&sequenceDATA->current_job->job_lock);

    sequenceDATA->current_job->job_state = sequenceDATA->current_job->job_state | mask;

    //pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
    
    #ifdef LOG_DATA
    clock_t time = clock();
    log_info(sequenceDATA->task->task_id, sequenceDATA->sequence_id, finished_node, sequenceDATA->current_job->job_id, time, NODE_EXECUTION_FINISHED);
    #endif
}

int TryExecuteNode(struct sequence_data *sequenceDATA, int node)
{
	
    pthread_mutex_lock(&sequenceDATA->current_job->job_lock);
     
    //Check if the next node can be executed
    if((check_precedence_constraints(sequenceDATA, node)) == false)
    {
        pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
        return 1;
    }
    
    //Check if node is not already being executed
    if((check_if_node_in_execution(node, sequenceDATA->current_job)) == true)
    {
        pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
        return 2;
    } 
    
    int mask = 1;
    mask = mask << (node - 1);
    sequenceDATA->current_job->job_execution_state = sequenceDATA->current_job->job_execution_state | mask;
    
    
    //Save clock cycles
    #ifdef LOG_DATA
	clock_t time = clock();
	#endif
	
	//Unlock
    pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
    
    //Log event
    #ifdef LOG_DATA
    log_info(sequenceDATA->task->task_id, sequenceDATA->sequence_id, node, sequenceDATA->current_job->job_id, time, NODE_EXECUTION_STARTED);
    #endif
    
    //Execute Node
    sequenceDATA->task->func[(node-1)]();
    
    //Mark node as finished
    finish_node(sequenceDATA, node);	
    
    #ifdef AUTO_SIGNAL
    //pthread_mutex_lock(&sequenceDATA->current_job->job_lock);
    SignalSequenceAut(node, sequenceDATA);
    //pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
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
	
	if(finished_node == sequenceDATA->task->number_of_nodes)
	{
		return;
	}
	
	for(int x = 0; x < (sequenceDATA->task->number_of_nodes -1); x++)
	{
		bool *ptr = sequenceDATA->task->precedence_matrix;
		
		int index = (finished_node -1) + (x * (sequenceDATA->task->number_of_nodes -1));
		
		ptr = ptr + index;
	
		
		if(*ptr == 1)
		{
			for(int i = 0; i < (sequenceDATA->task->number_of_sequences -1); i++)
			{

				if(*(sequenceDATA->task->sequence_heads+i) == (x+2))
				{
		
					bool flag = check_precedence_constraints(sequenceDATA, (x+2));
									
					if(flag == true)
					{
						sem_t *semaphore = sequenceDATA->current_job->secondary_sequences_guards + (i + 1);											
						sem_post(semaphore);   
					}
				}				
			}
		}				
	}
}

bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer)
{
    int mask = 1;
    mask = mask << (node_number - 1);
    u_int32_t local_execution_state = job_pointer->job_execution_state;
    
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
    clock_t time = clock();
    log_info(sequenceDATA->task->task_id, sequenceDATA->sequence_id, node, sequenceDATA->current_job->job_id, time, SEQUENCE_TERMINATED);
    #endif
}