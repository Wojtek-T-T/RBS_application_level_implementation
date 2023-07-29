#include "rbs_lib.h"


void InitializeSequence(struct task_data *taskDATA, int sequenceID, pthread_t *thread, sem_t *semaphore, void *(*func)())
{
	//Allocate memory for sequence data
	struct sequence_data *sequenceDATA = malloc(sizeof(struct sequence_data));
	
	sequenceDATA->task = taskDATA;
	sequenceDATA->sequence_id = sequenceID;
	sequenceDATA->semaphore = semaphore;
	sequenceDATA->current_job = taskDATA->last_added_job;
	
	//Initialize semaphore
	sem_init(semaphore, 0, 0);
	
	//Initialize thread
    pthread_create(thread, NULL, func, (void*) sequenceDATA);
 
}

void initialize_rbs()
{
	#ifdef LOG_DATA
	printf("log\n");
	openlog("RBS_IMPLEMENTATION_LOG", LOG_PID|LOG_CONS, LOG_USER);
	#endif
}

struct task_data *InitializeTask(int taskID, bool *precedenceMATRIX, int number_of_nodes, sem_t *semaphores_address, void (*workload[])())
{
	//allocate memory for the data structure
	struct task_data *taskDATA = malloc(sizeof(struct task_data));
	
	taskDATA->task_id = taskID;
	taskDATA->job_counter = 0;
	taskDATA->precedence_matrix = precedenceMATRIX;
	taskDATA->number_of_nodes = number_of_nodes;
	taskDATA->sem = semaphores_address;
	taskDATA->last_added_job = malloc(sizeof(struct job_token));
	taskDATA->last_added_job->previous_job = NULL;
	
	//Copy functions pointers
	for(int i = 0; i < number_of_nodes; i++)
	{
		taskDATA->func[i] = workload[i];
	}
	
	return taskDATA;
	
}


void log_info(int task, int sequence, int node, int job, clock_t time_stamp, int event)
{
	if(event == NODE_EXECUTION_STARTED)
	{
		syslog(LOG_INFO, "NODE_EXECUTION_STARTED, task %d, sequence %d, node %d, job %d at cycle: %f",task, sequence, node, job, (float)time_stamp);
	}
	else if(event == NODE_EXECUTION_FINISHED)
	{
		syslog(LOG_INFO, "NODE_EXECUTION_FINISHED, task %d, sequence %d, node %d, job %d at cycle: %f",task, sequence, node, job, (float)time_stamp);
	}
	else if(event == NEW_JOB_RELEASED)
	{
		syslog(LOG_INFO, "NEW_JOB_RELEASED, task %d, job %d at cycle: %f",task, job, (float)time_stamp);
	}
	else if(event == JOB_EXECUTION_FINISHED)
	{
		syslog(LOG_INFO, "JOB_EXECUTION_FINISHED, task %d, job %d at cycle: %f",task, job, (float)time_stamp);
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
    
    //initialize job data lock
    pthread_mutex_init(&new_job->job_lock, NULL);
    
    //update the pointer to the next job
    taskDATA->last_added_job->next_job = new_job;
    
    //post semaphore
    sem_post(taskDATA->sem);
    
    #ifdef LOG_DATA
    clock_t time = clock();
    log_info(taskDATA->task_id, 0, new_job->job_id, new_job->job_id, time, NEW_JOB_RELEASED);
    #endif
    
    //update the pointer to the last job
    taskDATA->last_added_job = new_job;
    
}

void finish_job(struct job_token *finished_job)
{
    pthread_mutex_destroy(&finished_job->job_lock);
    
    //Free memory
    free(finished_job->previous_job);
}

bool check_precedence_constraints(u_int8_t number_of_nodes, bool *precedence_matrix_pointer, u_int8_t node_number, struct job_token *job_pointer)
{
    //source nodes doesn't have any precedence constraints
    if(node_number == 1)
    {
        return true;
    }

    u_int8_t start_index = (node_number-2) * (number_of_nodes-1);
    u_int32_t mask = 0xFFFFFFFF;
    u_int32_t job_state_local = job_pointer->job_state & mask;
    bool *temp_bool_pointer = NULL;

    for(int x = 0; x < (number_of_nodes-1); x++)
    {
        u_int8_t temp_index = start_index + x;
        temp_bool_pointer = precedence_matrix_pointer + temp_index;

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

    pthread_mutex_lock(&sequenceDATA->current_job->job_lock);

    sequenceDATA->current_job->job_state = sequenceDATA->current_job->job_state | mask;

    pthread_mutex_unlock(&sequenceDATA->current_job->job_lock);
    
    #ifdef LOG_DATA
    clock_t time = clock();
    log_info(sequenceDATA->task->task_id, sequenceDATA->sequence_id, finished_node, sequenceDATA->current_job->job_id, time, NODE_EXECUTION_FINISHED);
    #endif
}

int TryExecuteNode(struct sequence_data *sequenceDATA, int node)
{
	
     pthread_mutex_lock(&sequenceDATA->current_job->job_lock);
     
     
    //Check if the next node can be executed
    if((check_precedence_constraints(sequenceDATA->task->number_of_nodes, sequenceDATA->task->precedence_matrix, node, sequenceDATA->current_job)) == false)
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
    
    return 0;
}

void signal_sequence_head(int node_to_signal, struct job_token *handled_job, sem_t *semaphore, int num_nodes, bool *precedence_matrix_pointer)
{
    bool signal_flag = check_precedence_constraints(num_nodes, precedence_matrix_pointer, node_to_signal, handled_job);

    if(signal_flag == true)
    {
        sem_post(semaphore);
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
