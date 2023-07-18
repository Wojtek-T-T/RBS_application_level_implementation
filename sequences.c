#include "sequences.h"


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


void *(*seq_func_ptr_t1[NR_SEQ_T1])() = {sequence_1_1_function, sequence_1_2_function, sequence_1_3_function, sequence_1_4_function};


void *sequence_1_1_function(void *arguments)
{
    set_cpu(0);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *current_job = NULL;
    bool *pre_pointer = task_1_precedence_constraints;
    

    while(true)
    {
        //wait for new job
        sem_wait(&task_1_semaphore);

        //Get pointer to the new job
        current_job = GetNewJob(old_job, queue_start, Q_SIZE);
        
        
         //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 1, pre_pointer, 7, 1) == false)
        {
            //Terminate sequence
            break;
        }
        
        
        //Start executing the first node
        node_1_1();
        finish_node(current_job, 1);

        
        //Signal other sequences
        signal_sequence_head(4, current_job, &task_1_sequence_3_semaphore, 7, pre_pointer);
        signal_sequence_head(3, current_job, &task_1_sequence_2_semaphore, 7, pre_pointer);
        
        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 2, pre_pointer, 7, 1) == false)
        {
            //Terminate sequence
            break;
        }
        

        //Execute node
        node_1_2();
        finish_node(current_job, 2);


        //Signal other sequences
        signal_sequence_head(6, current_job, &task_1_sequence_4_semaphore, 7, pre_pointer);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 5, pre_pointer, 7, 1) == false)
        {
            //Terminate sequence
            break;
        }


        //Execute node
        node_1_5();
        finish_node(current_job, 5);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 7, pre_pointer, 7, 1) == false)
        {
            //Terminate sequence
            break;
        }

        //Execute node
        node_1_7();
        finish_node(current_job, 7);
        
        break;

    }
    

}

void *sequence_1_2_function(void *arguments)
{
    set_cpu(1);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *current_job = NULL;
    bool *pre_pointer = task_1_precedence_constraints;

    while(true)
    {
        sem_wait(&task_1_sequence_2_semaphore);
        

        //Get pointer to the new job
        current_job = GetNewJob(old_job, queue_start, Q_SIZE);
        
       
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 3, pre_pointer, 7, 2) == false)
        {
            //Terminate sequence
            break;
        }
        node_1_3();
        finish_node(current_job, 3);
        

        //Signal other sequences
        signal_sequence_head(6, current_job, &task_1_sequence_4_semaphore, 7, pre_pointer);

        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 5, pre_pointer, 7, 2) == false)
        {
            //Terminate sequence
            break;
        }

        //Execute node
        node_1_5();
        finish_node(current_job, 5);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 7, pre_pointer, 7, 2) == false)
        {
            //Terminate sequence
            break;
        }

        node_1_7();
        finish_node(current_job, 7);
        
        break;


    }
    
}

void *sequence_1_3_function(void *arguments)
{
    set_cpu(2);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *current_job = NULL;
    bool *pre_pointer = task_1_precedence_constraints;

    while(true)
    {
        sem_wait(&task_1_sequence_3_semaphore);

        //Get pointer to the new job
        current_job = GetNewJob(old_job, queue_start, Q_SIZE);
        
        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 4, pre_pointer, 7, 3) == false)
        {
            //Terminate sequence
            break;
        }

        //Start executing the first node
        node_1_4();
        finish_node(current_job, 4);
        

        //Signal other sequences
        signal_sequence_head(6, current_job, &task_1_sequence_4_semaphore, 7, pre_pointer);

        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 5, pre_pointer, 7, 3) == false)
        {
            //Terminate sequence
            break;
        }
        

        //Execute node
        node_1_5();
        finish_node(current_job, 5);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 7, pre_pointer, 7, 3) == false)
        {
            //Terminate sequence
            break;
        }

        node_1_7();
        finish_node(current_job, 7);
        
        break;
    }

}

void *sequence_1_4_function(void *arguments)
{
    set_cpu(3);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *current_job = NULL;
    bool *pre_pointer = task_1_precedence_constraints;

    while(true)
    {
        sem_wait(&task_1_sequence_4_semaphore);

        //Get pointer to the new job
        current_job = GetNewJob(current_job, queue_start, Q_SIZE);
        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 6, pre_pointer, 7, 4) == false)
        {
            //Terminate sequence
            break;
        }

        //Start executing the first node
        node_1_6();
        finish_node(current_job, 6);

        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 7, pre_pointer, 7, 4) == false)
        {
            //Terminate sequence
            break;
        }
        
        node_1_7();
        finish_node(current_job, 7);
        
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
