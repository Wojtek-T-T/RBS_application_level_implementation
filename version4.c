#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <syslog.h>
#include <time.h>
#include <sched.h>
#include <syslog.h>

#define queue_size 3

#define task_1_nodes 7
#define task_2_nodes 5

bool task_1_precedence_constraints[36] = 
{
1,	0,	0,	0,	0,	0,
1,	0,	0,	0,	0,	0,
1,	0,	0,	0,	0,	0,
0,	1,	1,	1,	0,	0,
0,	1,	1,	1,	0,	0,
0,	0,	0,	0,	1,	1
};


bool task_2_precedence_constraints[16] = 
{
1,	0,	0,	0,
1,	0,	0,	0,
1,	0,	0,	0,
0,	1,	1,	1
};

volatile u_int32_t conv_array[10000];

volatile u_int32_t conv_array_t1_n1[10000];
volatile u_int32_t conv_array_t1_n2[10000];
volatile u_int32_t conv_array_t1_n3[10000];
volatile u_int32_t conv_array_t1_n4[10000];
volatile u_int32_t conv_array_t1_n5[10000];
volatile u_int32_t conv_array_t1_n6[10000];
volatile u_int32_t conv_array_t1_n7[10000];


//task data structures
struct job_token
{
    u_int32_t job_execution_state;
    u_int32_t job_state;
    pthread_mutex_t job_lock;

};

struct job_token task_1_jobs_queue[queue_size];
struct job_token task_2_jobs_queue[queue_size];


//semaphores
sem_t task_1_semaphore;
sem_t task_1_sequence_2_semaphore;
sem_t task_1_sequence_3_semaphore;
sem_t task_1_sequence_4_semaphore;

sem_t task_2_semaphore;
sem_t task_2_sequence_2_semaphore;
sem_t task_2_sequence_3_semaphore;


//condition variables
pthread_cond_t task_1_sequence_2_con = PTHREAD_COND_INITIALIZER;
pthread_cond_t task_1_sequence_3_con = PTHREAD_COND_INITIALIZER;
pthread_cond_t task_1_sequence_4_con = PTHREAD_COND_INITIALIZER;

//mutex locks
pthread_mutex_t task_1_sequence_2_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t task_1_sequence_3_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t task_1_sequence_4_mut = PTHREAD_MUTEX_INITIALIZER;


//prototypes
void *sequence_1_1_function(void *arguments);
void *sequence_1_2_function(void *arguments);
void *sequence_1_3_function(void *arguments);
void *sequence_1_4_function(void *arguments);

void *sequence_2_1_function(void *arguments);
void *sequence_2_2_function(void *arguments);
void *sequence_2_3_function(void *arguments);

struct job_token* GetNewJob(struct job_token *old_job_ptr, struct job_token *start_ptr);
void initialize_new_job(struct job_token *last_added_job_ptr, struct job_token *queue_start_ptr);
void initialize_semaphores();
void finish_job(struct job_token *finished_job);
bool check_precedence_constraints(u_int8_t number_of_nodes, bool *precedence_matrix_pointer, u_int8_t node_number, struct job_token *job_pointer);
void one_time_unit_workload(volatile u_int32_t *save_place);
void finish_node(struct job_token *handled_job, int finished_node);
bool claim_node(struct job_token *job, int claimed_node, bool *precedence_matrix_pointer, u_int8_t number_of_nodes, int seq);
void signal_sequence_head(int node_to_signal, struct job_token *handled_job, sem_t *semaphore, int num_nodes, bool *precedence_matrix_pointer);
void set_cpu(int cpu_num);
bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer);


void node_1_1();
void node_1_2();
void node_1_3();
void node_1_4();
void node_1_5();
void node_1_6();
void node_1_7();

void node_2_1();
void node_2_2();
void node_2_3();
void node_2_4();
void node_2_5();


clock_t start;


int main(void) 
{
    for(int p = 0; p <10000; p++)
    {

        conv_array[p] = p+18;
        
    }
    
    
    openlog("RBS_IMPLEMENTATION_LOG", LOG_PID|LOG_CONS, LOG_USER);
    initialize_semaphores();

    pthread_t task_1_sequence_1_thread;
    pthread_t task_1_sequence_2_thread;
    pthread_t task_1_sequence_3_thread;
    pthread_t task_1_sequence_4_thread;

    pthread_t task_2_sequence_1_thread;
    pthread_t task_2_sequence_2_thread;
    pthread_t task_2_sequence_3_thread;

    char *message1 = "thread 1 ";

    pthread_create( &task_1_sequence_1_thread, NULL, sequence_1_1_function, (void*) message1);
    pthread_create( &task_1_sequence_2_thread, NULL, sequence_1_2_function, (void*) message1);
    pthread_create( &task_1_sequence_3_thread, NULL, sequence_1_3_function, (void*) message1);
    pthread_create( &task_1_sequence_4_thread, NULL, sequence_1_4_function, (void*) message1);


    struct job_token *start_pointer = &task_1_jobs_queue[0];


    start = clock();

    initialize_new_job(NULL, start_pointer);
    
    
    sem_post(&task_1_semaphore);

    syslog(LOG_INFO, "Started job of task 1 at:, %f", (float)start);
    

    pthread_join(task_1_sequence_1_thread, NULL);
    pthread_join(task_1_sequence_2_thread, NULL);
    pthread_join(task_1_sequence_3_thread, NULL);
    pthread_join(task_1_sequence_4_thread, NULL);
        
    
    closelog();

}



void *sequence_1_1_function(void *arguments)
{
    set_cpu(0);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *handled_job = NULL;
    bool pre_flag= false;
    bool exe_flag = false;
    bool *pre_pointer = task_1_precedence_constraints;
    clock_t current_time;
    

    while(true)
    {
        //wait for new job
        sem_wait(&task_1_semaphore);

        //Get pointer to the new job
        handled_job = GetNewJob(old_job, queue_start);
        
        
        //Start executing the first node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 1, sequence 1 at: %f", (float)current_time);
        node_1_1();
        finish_node(handled_job, 1);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 1, sequence 1 at: %f", (float)current_time);
        
        
        

        //Signal other sequences
        signal_sequence_head(4, handled_job, &task_1_sequence_3_semaphore, 7, pre_pointer);
        signal_sequence_head(3, handled_job, &task_1_sequence_2_semaphore, 7, pre_pointer);
        
        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(handled_job, 2, pre_pointer, 7, 1) == false)
        {
            //Terminate sequence
            break;
        }
        

        //Execute node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 2, sequence 1 at: %f", (float)current_time);
        node_1_2();
        finish_node(handled_job, 2);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 2, sequence 1 at: %f", (float)current_time);


        //Signal other sequences
        signal_sequence_head(6, handled_job, &task_1_sequence_4_semaphore, 7, pre_pointer);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(handled_job, 5, pre_pointer, 7, 1) == false)
        {
            //Terminate sequence
            break;
        }


        //Execute node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 5, sequence 1 at: %f", (float)current_time);
        node_1_5();
        finish_node(handled_job, 5);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 5, sequence 1 at: %f", (float)current_time);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(handled_job, 7, pre_pointer, 7, 1) == false)
        {
            //Terminate sequence
            break;
        }

        //Execute node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 7, sequence 1 at: %f", (float)current_time);
        node_1_7();
        finish_node(handled_job, 7);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 7, sequence 1 at: %f", (float)current_time);
        
        break;

    }
    

}

void *sequence_1_2_function(void *arguments)
{
    set_cpu(1);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *handled_job = NULL;
    bool pre_flag= false;
    bool exe_flag = false;
    bool *pre_pointer = task_1_precedence_constraints;
    clock_t current_time;

    while(true)
    {
        sem_wait(&task_1_sequence_2_semaphore);
        

        //Get pointer to the new job
        handled_job = GetNewJob(old_job, queue_start);

        //Start executing the first node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 3, sequence 2 at: %f", (float)current_time);
        node_1_3();
        finish_node(handled_job, 3);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 3, sequence 2 at: %f", (float)current_time);

        //Signal other sequences
        signal_sequence_head(6, handled_job, &task_1_sequence_4_semaphore, 7, pre_pointer);

        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(handled_job, 5, pre_pointer, 7, 2) == false)
        {
            //Terminate sequence
            break;
        }


        //Execute node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 5, sequence 2 at: %f", (float)current_time);
        node_1_5();
        finish_node(handled_job, 5);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 5, sequence 2 at: %f", (float)current_time);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(handled_job, 7, pre_pointer, 7, 2) == false)
        {
            //Terminate sequence
            break;
        }

        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 7, sequence 2 at: %f", (float)current_time);
        node_1_7();
        finish_node(handled_job, 7);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 7, sequence 2 at: %f", (float)current_time);
        
        break;


    }
    
}

void *sequence_1_3_function(void *arguments)
{
    set_cpu(2);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *handled_job = NULL;
    bool pre_flag= false;
    bool exe_flag = false;
    bool *pre_pointer = task_1_precedence_constraints;
    clock_t current_time;

    while(true)
    {
        sem_wait(&task_1_sequence_3_semaphore);

        //Get pointer to the new job
        handled_job = GetNewJob(old_job, queue_start);

        //Start executing the first node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 4, sequence 3 at: %f", (float)current_time);
        node_1_4();
        finish_node(handled_job, 4);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 4, sequence 3 at: %f", (float)current_time);
        

        //Signal other sequences
        signal_sequence_head(6, handled_job, &task_1_sequence_4_semaphore, 7, pre_pointer);

        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(handled_job, 5, pre_pointer, 7, 3) == false)
        {
            //Terminate sequence
            break;
        }
        

        //Execute node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 5, sequence 3 at: %f", (float)current_time);
        node_1_5();
        finish_node(handled_job, 5);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 5, sequence 3 at: %f", (float)current_time);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(handled_job, 7, pre_pointer, 7, 3) == false)
        {
            //Terminate sequence
            break;
        }

        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 7, sequence 3 at: %f", (float)current_time);
        node_1_7();
        finish_node(handled_job, 7);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 7, sequence 3 at: %f", (float)current_time);
        
        break;
    }

}

void *sequence_1_4_function(void *arguments)
{
    set_cpu(3);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *handled_job = NULL;
    bool pre_flag= false;
    bool exe_flag = false;
    bool *pre_pointer = task_1_precedence_constraints;
    clock_t current_time;

    while(true)
    {
        sem_wait(&task_1_sequence_4_semaphore);

        //Get pointer to the new job
        handled_job = GetNewJob(old_job, queue_start);

        //Start executing the first node
        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 6, sequence 4 at: %f", (float)current_time);
        node_1_6();
        finish_node(handled_job, 6);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 6, sequence 4 at: %f", (float)current_time);

        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(handled_job, 7, pre_pointer, 7, 4) == false)
        {
            //Terminate sequence
            break;
        }
        

        current_time = clock() - start;
        syslog(LOG_INFO, "Started execution of task 1, node 7, sequence 4 at: %f", (float)current_time);
        node_1_7();
        finish_node(handled_job, 7);
        current_time = clock() - start;
        syslog(LOG_INFO, "Finished execution of task 1, node 7, sequence 4 at: %f", (float)current_time);
        
        break;
    }
}

void *sequence_2_1_function(void *arguments)
{


    while(true)
    {
        sem_wait(&task_2_semaphore);
    }
    

}

void *sequence_2_2_function(void *arguments)
{
    while(true)
    {
        sem_wait(&task_2_sequence_2_semaphore);
    }
}

void *sequence_2_3_function(void *arguments)
{
    while(true)
    {
        sem_wait(&task_2_sequence_3_semaphore);
    }

}


struct job_token* GetNewJob(struct job_token *old_job_ptr, struct job_token *start_ptr)
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

void initialize_new_job(struct job_token *last_added_job_ptr, struct job_token *queue_start_ptr)
{
    struct job_token *new_job = NULL;
    if(last_added_job_ptr == NULL)
    {
        new_job = queue_start_ptr;
    }
    else if((queue_start_ptr + (queue_size - 1)) == last_added_job_ptr)
    {
        new_job = queue_start_ptr;
    }
    else
    {
        new_job = last_added_job_ptr + 1;
    }

    new_job->job_execution_state = 0;
    new_job->job_state = 0;
    pthread_mutex_init(&new_job->job_lock, NULL);
}

void finish_job(struct job_token *finished_job)
{
    pthread_mutex_destroy(&finished_job->job_lock);
}

void initialize_semaphores()
{
    sem_init(&task_1_semaphore, 0, 0);
    sem_init(&task_1_sequence_2_semaphore, 0, 0);
    sem_init(&task_1_sequence_3_semaphore, 0, 0);
    sem_init(&task_1_sequence_4_semaphore, 0, 0);

    sem_init(&task_2_semaphore, 0, 0);
    sem_init(&task_2_sequence_2_semaphore, 0, 0);
    sem_init(&task_2_sequence_3_semaphore, 0, 0);
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

void one_time_unit_workload(volatile u_int32_t *save_place)
{
    volatile u_int32_t *ptr = NULL;
    
    
    for(int p = 0; p < 10000; p++)
    {
        int temp_value = 0;
        
        
        for(int small_ind = 0; small_ind <10; small_ind++)
        {
            
            if(small_ind == 0)
            {
                temp_value = temp_value + conv_array[p];
            }
            else
            {
                int index = p - small_ind;
                if(index >= 0)
                {
                    temp_value = temp_value + conv_array[index];
                }
            
                index = p + small_ind;
            
                if(index <= 9999)
                {
                    temp_value = temp_value + conv_array[index];
                }
                
            } 
            
            
        }
        
        
        temp_value = temp_value/19;
        
        ptr = save_place + p;
        
        *ptr = temp_value;
        
    }
}

void finish_node(struct job_token *handled_job, int finished_node)
{
    int mask = 1;
    mask = mask << (finished_node - 1);

    pthread_mutex_lock(&handled_job->job_lock);

    handled_job->job_state = handled_job->job_state | mask;

    pthread_mutex_unlock(&handled_job->job_lock);
}

bool claim_node(struct job_token *job, int claimed_node, bool *precedence_matrix_pointer, u_int8_t number_of_nodes, int seq)
{
    
     pthread_mutex_lock(&job->job_lock);
     
     
    
    //Check if the next node can be executed
    if((check_precedence_constraints(number_of_nodes, precedence_matrix_pointer, claimed_node, job)) == false)
    {
        pthread_mutex_unlock(&job->job_lock);
        //printf("job stste, node %d , seq: %d: %d = terminates\n", claimed_node, seq, job->job_state);
        return false;
    }
    
    
    if((check_if_node_in_execution(claimed_node, job)) == true)
    {
        pthread_mutex_unlock(&job->job_lock);
        //printf("job stste, node %d, seq: %d : %d, ex state : %d = terminates\n", claimed_node, seq, job->job_state, job->job_execution_state);
        return false;
    }
    
    
    int mask = 1;
    mask = mask << (claimed_node - 1);
    
    
    //pthread_mutex_lock(&job->job_lock);

    job->job_execution_state = job->job_execution_state | mask;
    
    //printf("job stste, node %d, seq: %d : %d, ex state : %d = continues\n", claimed_node, seq, job->job_state, job->job_execution_state);

    pthread_mutex_unlock(&job->job_lock);

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

void set_cpu(int cpu_num)
{
   //int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(cpu_num, &cpuset);  
   sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}


void node_1_1()
{
    volatile u_int32_t *ptr = &conv_array_t1_n1[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_2()
{
    volatile u_int32_t *ptr = &conv_array_t1_n2[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_3()
{
    volatile u_int32_t *ptr = &conv_array_t1_n3[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_4()
{
    volatile u_int32_t *ptr = &conv_array_t1_n4[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_5()
{
    volatile u_int32_t *ptr = &conv_array_t1_n5[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_6()
{
    volatile u_int32_t *ptr = &conv_array_t1_n6[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_7()
{
    volatile u_int32_t *ptr = &conv_array_t1_n7[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_2_1()
{

}

void node_2_2()
{

}

void node_2_3()
{

}

void node_2_4()
{

}

void node_2_5()
{

}
