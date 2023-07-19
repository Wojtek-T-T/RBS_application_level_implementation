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
#include "sequences.h"

void initialize_semaphores();


int main(void) 
{
    struct timespec tim;
    tim.tv_sec  = 1;
    tim.tv_nsec = 500000000L;
    
    
    //Create an array for convolution
    create_workload();
    
    //Initialize RBS
    initialize_rbs();
    
    //Initialize the semaphores guarding the sequences
    initialize_semaphores();



    char *message1 = "thread 1 ";

    pthread_create( &task_1_sequence_1_thread, NULL, seq_func_ptr_t1[0], (void*) message1);
    pthread_create( &task_1_sequence_2_thread, NULL, sequence_1_2_function, (void*) message1);
    pthread_create( &task_1_sequence_3_thread, NULL, sequence_1_3_function, (void*) message1);
    pthread_create( &task_1_sequence_4_thread, NULL, sequence_1_4_function, (void*) message1);


    struct job_token *start_pointer = &task_1_jobs_queue[0];


    initialize_new_job(NULL, start_pointer, Q_SIZE, 1);
    sem_post(&task_1_semaphore);
    
    
    int bla = nanosleep(&tim , &tim);
    
    initialize_new_job(start_pointer, start_pointer, Q_SIZE, 1);
    sem_post(&task_1_semaphore);
    
    int bla = nanosleep(&tim , &tim);
    
    initialize_new_job((start_pointer +1), start_pointer, Q_SIZE, 1);
    sem_post(&task_1_semaphore);
    

    pthread_join(task_1_sequence_1_thread, NULL);
    pthread_join(task_1_sequence_2_thread, NULL);
    pthread_join(task_1_sequence_3_thread, NULL);
    pthread_join(task_1_sequence_4_thread, NULL);
        
    
    closelog();

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


