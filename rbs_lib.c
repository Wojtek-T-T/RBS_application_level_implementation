#include "rbs_lib.h"


void initialize_rbs()
{
	openlog("RBS_IMPLEMENTATION_LOG", LOG_PID|LOG_CONS, LOG_USER);
}

void initialize_task(int number_of_nodes, int number_of_sequences, sem_t *semaphores, pthread_t *threads)
{
	char *message1 = "thread 1 ";
	
	for(int i = 0; i++; i < number_of_sequences)
	{
		sem_t *sem_ptr = semaphores + i;
		sem_init(sem_ptr, 0, 0);
		
		
		//pthread_create( &task_1_sequence_1_thread, NULL, sequence_1_1_function, (void*) message1);
		
	}
	
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



struct job_token* GetNewJob(struct job_token *old_job_ptr, struct job_token *start_ptr, int queue_size)
{
    if(old_job_ptr == NULL)
    {
        return start_ptr;
    }


    struct job_token *new_job = NULL;
    if((start_ptr + (queue_size - 1)) == old_job_ptr)
    {
        new_job = start_ptr;
    }
    else
    {
        new_job = old_job_ptr + 1;
    }
    return new_job;
}

void initialize_new_job(struct job_token *last_added_job_ptr, struct job_token *queue_start_ptr, int queue_size, int task_id)
{
    struct job_token *new_job = NULL;
    if(last_added_job_ptr == NULL)
    {
        new_job = queue_start_ptr;
        new_job->job_id = 0;
    }
    else if((queue_start_ptr + (queue_size - 1)) == last_added_job_ptr)
    {
        new_job = queue_start_ptr;
        new_job->job_id = last_added_job_ptr->job_id + 1;
    }
    else
    {
        new_job = last_added_job_ptr + 1;
    }

	new_job->task_id = task_id;
    new_job->job_execution_state = 0;
    new_job->job_state = 0;
    pthread_mutex_init(&new_job->job_lock, NULL);
    
    clock_t time = clock();
    log_info(new_job->task_id, 0, 0, new_job->job_id, time, NEW_JOB_RELEASED);
}

void finish_job(struct job_token *finished_job)
{
    pthread_mutex_destroy(&finished_job->job_lock);
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

void finish_node(struct job_token *job, int finished_node)
{
    int mask = 1;
    mask = mask << (finished_node - 1);

    pthread_mutex_lock(&job->job_lock);

    job->job_state = job->job_state | mask;

    pthread_mutex_unlock(&job->job_lock);
    
    clock_t time = clock();
    log_info(job->task_id, 0, finished_node, job->job_id, time, NODE_EXECUTION_FINISHED);
}

bool claim_node(struct job_token *job, int claimed_node, bool *precedence_matrix_pointer, u_int8_t number_of_nodes, int seq)
{
    
     pthread_mutex_lock(&job->job_lock);
     
     
    //Check if the next node can be executed
    if((check_precedence_constraints(number_of_nodes, precedence_matrix_pointer, claimed_node, job)) == false)
    {
        pthread_mutex_unlock(&job->job_lock);
        return false;
    }
    
    /*
    if((check_if_node_in_execution(claimed_node, job)) == true)
    {
        pthread_mutex_unlock(&job->job_lock);
        return false;
    } */
    
    
    //int mask = 1;
    //mask = mask << (claimed_node - 1);
    //job->job_execution_state = job->job_execution_state | mask;
    

    pthread_mutex_unlock(&job->job_lock);
    
    clock_t time = clock();
    log_info(job->task_id, seq, claimed_node, job->job_id, time, NODE_EXECUTION_STARTED);
    
    return true;
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
