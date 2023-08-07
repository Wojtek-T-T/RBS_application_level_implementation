#include "sequences.h"

int main(void) 
{    
    struct timespec tim;
    tim.tv_sec  = 1;
    tim.tv_nsec = 500000000L;
    
    //Create an array for convolution
    create_workload();
    

    //Initialize RBS
    initialize_rbs();
    

    //Initialize tasks
    InitializeTask(&task1_data);
    InitializeTask(&task2_data);
    

    //initialize sequences
    InitializeSequence(&task1_data, 1, &task_1_sequence_1_thread, task1_data.attr, seq_func_ptr_t1[0]);
    InitializeSequence(&task1_data, 2, &task_1_sequence_2_thread, task1_data.attr, seq_func_ptr_t1[1]);
    InitializeSequence(&task1_data, 3, &task_1_sequence_3_thread, task1_data.attr, seq_func_ptr_t1[2]);
    InitializeSequence(&task1_data, 4, &task_1_sequence_4_thread, task1_data.attr, seq_func_ptr_t1[3]);
    
    InitializeSequence(&task2_data, 1, &task_2_sequence_1_thread, task2_data.attr, seq_func_ptr_t2[0]);
    InitializeSequence(&task2_data, 2, &task_2_sequence_2_thread, task2_data.attr, seq_func_ptr_t2[1]);
    InitializeSequence(&task2_data, 3, &task_2_sequence_3_thread, task2_data.attr, seq_func_ptr_t2[2]);   


    //printf("sem address: %d\n", &semaphores_T1[3]);
    

    //syslog(LOG_INFO, "SPACE SPACE SPACE SPACE");
    //syslog(LOG_INFO, "SPACE SPACE SPACE SPACE");
    
    //int result = nanosleep(&tim , &tim);
    
    ReleaseNewJob(&task1_data);
    //ReleaseNewJob(&task2_data);

    //result = nanosleep(&tim , &tim);
    
    ReleaseNewJob(&task1_data);
    //ReleaseNewJob(&task2_data);
    
    //result = nanosleep(&tim , &tim);
    
    
    ReleaseNewJob(&task1_data);
    //ReleaseNewJob(&task2_data);

    //result = nanosleep(&tim , &tim);
    
    
    ReleaseNewJob(&task1_data);
   // ReleaseNewJob(&task2_data);

    //result = nanosleep(&tim , &tim);

    ReleaseNewJob(&task1_data);
   // ReleaseNewJob(&task2_data);
    
    
    pthread_join(task_1_sequence_1_thread, NULL);
    pthread_join(task_1_sequence_2_thread, NULL);
    pthread_join(task_1_sequence_3_thread, NULL);
    pthread_join(task_1_sequence_4_thread, NULL);
    
    closelog();

}


