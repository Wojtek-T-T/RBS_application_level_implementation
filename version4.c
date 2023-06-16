#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#define queue_size 3

#define task_1_nodes 7
#define task_2_nodes 5

bool task_1_precedence_constraints[36] = 
{
1,	0,	0,	0,	0,	0,
1,	0,	0,	0,	1,	1,
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

//task data structures
struct job_token
{
    u_int32_t job_execution_state;
    pthread_mutex_t job_lock;
    bool status;
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
void one_time_unit_workload();



int main(void) 
{
    initialize_semaphores();

    pthread_t task_1_sequence_1_thread;
    pthread_t task_1_sequence_2_thread;
    pthread_t task_1_sequence_3_thread;
    pthread_t task_1_sequence_4_thread;

    pthread_t task_2_sequence_1_thread;
    pthread_t task_2_sequence_2_thread;
    pthread_t task_2_sequence_3_thread;

    char *message1 = "thread 1 ";

    /*
    pthread_create( &task_1_sequence_1_thread, NULL, sequence_1_1_function, (void*) message1);
    pthread_create( &task_1_sequence_2_thread, NULL, sequence_1_2_function, (void*) message1);
    pthread_create( &task_1_sequence_3_thread, NULL, sequence_1_3_function, (void*) message1);
    pthread_create( &task_1_sequence_4_thread, NULL, sequence_1_4_function, (void*) message1);

    pthread_create( &task_2_sequence_1_thread, NULL, sequence_2_1_function, (void*) message1);
    pthread_create( &task_2_sequence_2_thread, NULL, sequence_2_2_function, (void*) message1);
    pthread_create( &task_2_sequence_3_thread, NULL, sequence_2_3_function, (void*) message1);
    */

    /*
    task_1_jobs_queue[0].job_execution_state = 0;
    task_1_jobs_queue[1].job_execution_state = 1;
    task_1_jobs_queue[2].job_execution_state = 2;
    //task_1_jobs_queue[3].job_execution_state = 3;

    struct job_token *current_job = &task_1_jobs_queue[0];

    struct job_token *start_pointer = &task_1_jobs_queue[0];

    printf("%d\n", current_job->job_execution_state);

    current_job = GetNewJob(current_job, start_pointer);

    printf("%d\n", current_job->job_execution_state);

    current_job = GetNewJob(current_job, start_pointer);

    printf("%d\n", current_job->job_execution_state);

    current_job = GetNewJob(current_job, start_pointer);

    printf("%d\n", current_job->job_execution_state);

    current_job = GetNewJob(current_job, start_pointer);

    printf("%d\n", current_job->job_execution_state);
    */

    /*
    struct job_token *start_pointer = &task_1_jobs_queue[0];

    task_1_jobs_queue[0].job_execution_state = 999;
    task_1_jobs_queue[1].job_execution_state = 999;
    task_1_jobs_queue[2].job_execution_state = 999;

    for(int b = 0; b < 3; b++)
    {
        printf("value of queue place %d is %d\n", b, task_1_jobs_queue[b].job_execution_state);
    }

    initialize_new_job(NULL, start_pointer);

    for(int b = 0; b < 3; b++)
    {
        printf("value of queue place %d is %d\n", b, task_1_jobs_queue[b].job_execution_state);
    }

    initialize_new_job((start_pointer), start_pointer);

    for(int b = 0; b < 3; b++)
    {
        printf("value of queue place %d is %d\n", b, task_1_jobs_queue[b].job_execution_state);
    }

    task_1_jobs_queue[0].job_execution_state = 999;
    task_1_jobs_queue[1].job_execution_state = 999;
    task_1_jobs_queue[2].job_execution_state = 999;

    initialize_new_job((start_pointer+2), start_pointer);

    for(int b = 0; b < 3; b++)
    {
        printf("value of queue place %d is %d\n", b, task_1_jobs_queue[b].job_execution_state);
    }
    */

    struct job_token *start_pointer = &task_1_jobs_queue[0];

    initialize_new_job(NULL, start_pointer);

    start_pointer->job_execution_state = 14;

    bool *pre_pointer = task_1_precedence_constraints;

    bool bla = check_precedence_constraints(7, pre_pointer, 5, start_pointer);

    printf(" response is %d\n", bla);

}



void *sequence_1_1_function(void *arguments)
{
    sem_wait(&task_1_semaphore);
    

}

void *sequence_1_2_function(void *arguments)
{
    sem_wait(&task_1_sequence_2_semaphore);
}

void *sequence_1_3_function(void *arguments)
{
    sem_wait(&task_1_sequence_3_semaphore);

}

void *sequence_1_4_function(void *arguments)
{
    sem_wait(&task_1_sequence_4_semaphore);
}

void *sequence_2_1_function(void *arguments)
{
    sem_wait(&task_2_semaphore);

}

void *sequence_2_2_function(void *arguments)
{
    sem_wait(&task_2_sequence_2_semaphore);
}

void *sequence_2_3_function(void *arguments)
{
    sem_wait(&task_2_sequence_3_semaphore);

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
    u_int32_t job_state_local = job_pointer->job_execution_state & mask;
    bool *temp_bool_pointer = NULL;

    for(int x = 0; x < (number_of_nodes-2); x++)
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

void one_time_unit_workload()
{
    for (int i = 0; i < (0xFFFFFFF); i++);
}
