#include "sequences.h"

int main(void) 
{
    //int i, policy;
    pthread_attr_t attrT1;
    struct sched_param schedParamT1;
    pthread_attr_t attrT2;
    struct sched_param schedParamT2;

    pthread_attr_init(&attrT1);
    pthread_attr_init(&attrT2);
    
    
    //T1
    int result = pthread_attr_setschedpolicy(&attrT1, SCHED_FIFO);
    
    if(result != 0)
    {
        printf("sched policy set error\n");
    }
        
    schedParamT1.sched_priority = 50;
    
    result = pthread_attr_setschedparam(&attrT1, &schedParamT1);
    
    if(result != 0)
    {
        printf("sched param set error\n");
    }
        
    result = pthread_attr_setinheritsched(&attrT1, PTHREAD_EXPLICIT_SCHED);
    
    if(result != 0)
    {
        printf("sched set inherit  error\n");
    }
    
    //T2
    result = pthread_attr_setschedpolicy(&attrT2, SCHED_FIFO);
    
    if(result != 0)
    {
        printf("sched policy set error\n");
    }
        
    schedParamT2.sched_priority = 5;
    
    result = pthread_attr_setschedparam(&attrT2, &schedParamT2);
    
    if(result != 0)
    {
        printf("sched param set error\n");
    }
        
    result = pthread_attr_setinheritsched(&attrT2, PTHREAD_EXPLICIT_SCHED);
    
    if(result != 0)
    {
        printf("sched set inherit  error\n");
    }
    
    
    
    
    
    struct timespec tim;
    tim.tv_sec  = 1;
    tim.tv_nsec = 500000000L;
    
    
    struct task_data *task1_data;
    struct task_data *task2_data;
    
    //Create an array for convolution
    create_workload();
    
    //Initialize RBS
    initialize_rbs();
    
    //Initialize tasks
    task1_data = InitializeTask(1, task_1_precedence_constraints, task_1_sequence_heads, NR_NODES_T1, NR_SEQ_T1, semaphores_T1, nodes_func_ptr_t1);
    task2_data = InitializeTask(2, task_2_precedence_constraints, task_2_sequence_heads, NR_NODES_T2, NR_SEQ_T2, semaphores_T2, nodes_func_ptr_t2);
    
    //initialize sequences
    InitializeSequence(task1_data, 1, &task_1_sequence_1_thread, attrT1, seq_func_ptr_t1[0]);
    InitializeSequence(task1_data, 2, &task_1_sequence_2_thread, attrT1, seq_func_ptr_t1[1]);
    InitializeSequence(task1_data, 3, &task_1_sequence_3_thread, attrT1, seq_func_ptr_t1[2]);
    InitializeSequence(task1_data, 4, &task_1_sequence_4_thread, attrT1, seq_func_ptr_t1[3]);
    
    
    InitializeSequence(task2_data, 1, &task_2_sequence_1_thread, attrT2, seq_func_ptr_t2[0]);
    InitializeSequence(task2_data, 2, &task_2_sequence_2_thread, attrT2, seq_func_ptr_t2[1]);
    InitializeSequence(task2_data, 3, &task_2_sequence_3_thread, attrT2, seq_func_ptr_t2[2]);   
    
    
    
    ReleaseNewJob(task1_data);
    ReleaseNewJob(task2_data);
    
    
    /*
    
    int bla = nanosleep(&tim , &tim);
    
    ReleaseNewJob(task1_data);
    
    bla = nanosleep(&tim , &tim);
    
    
    ReleaseNewJob(task1_data);
    */
    
    pthread_join(task_1_sequence_1_thread, NULL);
    pthread_join(task_1_sequence_2_thread, NULL);
    pthread_join(task_1_sequence_3_thread, NULL);
    pthread_join(task_1_sequence_4_thread, NULL);
        
    
    closelog();

}


