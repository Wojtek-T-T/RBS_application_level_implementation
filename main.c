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
    
    
    struct task_data task1_data;
    struct sequence_data t1_s1_data;
    struct sequence_data t1_s2_data;
    struct sequence_data t1_s3_data;
    struct sequence_data t1_s4_data;
    
    
    
    //Create an array for convolution
    create_workload();
    
    //Initialize RBS
    initialize_rbs();
    
    //Initialize task 1
    InitializeTask(&task1_data, 1, NR_NODES_T1, Q_SIZE, &task_1_jobs_queue[0], task_1_precedence_constraints, &task_1_semaphore, nodes_func_ptr_t1);
    
    //initialize sequences
    InitializeSequence(&task1_data, &t1_s1_data, 1, &task_1_sequence_1_thread, &task_1_semaphore, seq_func_ptr_t1[0]);
    InitializeSequence(&task1_data, &t1_s2_data, 2, &task_1_sequence_2_thread, &task_1_sequence_2_semaphore, seq_func_ptr_t1[1]);
    InitializeSequence(&task1_data, &t1_s3_data, 3, &task_1_sequence_3_thread, &task_1_sequence_3_semaphore, seq_func_ptr_t1[2]);
    InitializeSequence(&task1_data, &t1_s4_data, 4, &task_1_sequence_4_thread, &task_1_sequence_4_semaphore, seq_func_ptr_t1[3]);
    
    ReleaseNewJob(&task1_data);

    
    /*
    
    
    int bla = nanosleep(&tim , &tim);
    
    initialize_new_job(start_pointer, start_pointer, Q_SIZE, 1, 2);
    sem_post(&task_1_semaphore);
    
    
    bla = nanosleep(&tim , &tim);
    
    
    initialize_new_job(start_pointer + 1, start_pointer, Q_SIZE, 1, 3);
    sem_post(&task_1_semaphore);
    
    
    bla = nanosleep(&tim , &tim);
    
    initialize_new_job(start_pointer + 2, start_pointer, Q_SIZE, 1, 4);
    sem_post(&task_1_semaphore);
    
    */
    

    pthread_join(task_1_sequence_1_thread, NULL);
    pthread_join(task_1_sequence_2_thread, NULL);
    pthread_join(task_1_sequence_3_thread, NULL);
    pthread_join(task_1_sequence_4_thread, NULL);
        
    
    closelog();

}


