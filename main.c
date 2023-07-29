#define _GNU_SOURCE 
#include <stdio.h>


/* DEFINE SETTINGS
 * LOG_EVENTS flag: events will be logged
 * SIGNAL_AUTOMATICALLY flag: sequences will be signalled automatically after all precedence constraints are fullfilled
 *                            otherwise manually signalling needed 
 * 
 */
#define LOG_DATA
//#define SIGNAL_AUTOMATICALLY 

#include "sequences.h"



int main(void) 
{
    struct timespec tim;
    tim.tv_sec  = 1;
    tim.tv_nsec = 500000000L;
    
    
    struct task_data *task1_data;
    
    //Create an array for convolution
    create_workload();
    
    //Initialize RBS
    initialize_rbs();
    
    //Initialize task 1
    task1_data = InitializeTask(1, task_1_precedence_constraints, NR_NODES_T1, &task_1_semaphore, nodes_func_ptr_t1);
    
    //initialize sequences
    InitializeSequence(task1_data, 1, &task_1_sequence_1_thread, &task_1_semaphore, seq_func_ptr_t1[0]);
    InitializeSequence(task1_data, 2, &task_1_sequence_2_thread, &task_1_sequence_2_semaphore, seq_func_ptr_t1[1]);
    InitializeSequence(task1_data, 3, &task_1_sequence_3_thread, &task_1_sequence_3_semaphore, seq_func_ptr_t1[2]);
    InitializeSequence(task1_data, 4, &task_1_sequence_4_thread, &task_1_sequence_4_semaphore, seq_func_ptr_t1[3]);
    
    ReleaseNewJob(task1_data);

    
    
    int bla = nanosleep(&tim , &tim);
    
    ReleaseNewJob(task1_data);
    
    bla = nanosleep(&tim , &tim);
    
    
    ReleaseNewJob(task1_data);
    

    pthread_join(task_1_sequence_1_thread, NULL);
    pthread_join(task_1_sequence_2_thread, NULL);
    pthread_join(task_1_sequence_3_thread, NULL);
    pthread_join(task_1_sequence_4_thread, NULL);
        
    
    closelog();

}


