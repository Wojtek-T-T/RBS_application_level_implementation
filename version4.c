#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#define queue_size 3

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

struct job_token * GetNewJob();
void initialize_new_job();
void initialize_semaphores();
void finish_job();
bool check_precedence_constraints();
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

void initialize_new_job()
{
    
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

void finish_job()
{

}

bool check_precedence_constraints()
{

}

void one_time_unit_workload()
{

}