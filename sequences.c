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
    bool *precedence_matrix = task_1_precedence_constraints;
    

    while(true)
    {
		
        //Get pointer to the new job
        current_job = GetNewJob(old_job, queue_start, Q_SIZE, &task_1_semaphore);
        
        
         //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 1, precedence_matrix, 7, 1) == true)
        {
			//Start executing the first node
			node_1_1();
			finish_node(current_job, 1, 1);			
        }
        else
        {
            //Terminate sequence
            old_job = current_job;
            continue;		
		}
        
      
        //Signal other sequences
        signal_sequence_head(4, current_job, &task_1_sequence_3_semaphore, 7, precedence_matrix);
        signal_sequence_head(3, current_job, &task_1_sequence_2_semaphore, 7, precedence_matrix);
        
        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 2, precedence_matrix, 7, 1) == true)
        {
			//Execute node
			node_1_2();
			finish_node(current_job, 2, 1);			
        }
        else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
		}


        //Signal other sequences
        signal_sequence_head(6, current_job, &task_1_sequence_4_semaphore, 7, precedence_matrix);


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 5, precedence_matrix, 7, 1) == true)
        {
			//Execute node
			node_1_5();
			finish_node(current_job, 5, 1);			
        }
        else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
		}	
		



        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 7, precedence_matrix, 7, 1) == true)
        {
			//Execute node
			node_1_7();
			finish_node(current_job, 7, 1);			
        }
        else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
		}
		
		old_job = current_job;

    }
    

}

void *sequence_1_2_function(void *arguments)
{
	//Set CPU
    set_cpu(1);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *current_job = NULL;
    bool *precedence_matrix = task_1_precedence_constraints;

    while(true)
    {
        //Get pointer to the new job
        current_job = GetNewJob(old_job, queue_start, Q_SIZE, &task_1_sequence_2_semaphore);
        
       
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 3, precedence_matrix, 7, 2) == true)
        {
			//Execute node
			node_1_3();
			finish_node(current_job, 3, 2);
		}
		else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
        }
        

        //Signal other sequences
        signal_sequence_head(6, current_job, &task_1_sequence_4_semaphore, 7, precedence_matrix);

        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 5, precedence_matrix, 7, 2) == true)
        {
			//Execute node
			node_1_5();
			finish_node(current_job, 5, 2);
		}
		else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
        }


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 7, precedence_matrix, 7, 2) == true)
        {
			//Execute node
			node_1_7();
			finish_node(current_job, 7, 2);
		}
		else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
        }
        
		old_job = current_job;

    }
    
}

void *sequence_1_3_function(void *arguments)
{
    set_cpu(2);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *current_job = NULL;
    bool *precedence_matrix = task_1_precedence_constraints;

    while(true)
    {

        //Get pointer to the new job
        current_job = GetNewJob(old_job, queue_start, Q_SIZE, &task_1_sequence_3_semaphore);
        
        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 4, precedence_matrix, 7, 3) == true)
        {
			//Execute node
			node_1_4();
			finish_node(current_job, 4, 3);
		}
		else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
        }
        

        //Signal other sequences
        signal_sequence_head(6, current_job, &task_1_sequence_4_semaphore, 7, precedence_matrix);

        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 5, precedence_matrix, 7, 3) == true)
        {
			//Execute node
			node_1_5();
			finish_node(current_job, 5, 3);
		}
		else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
        }


        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 7, precedence_matrix, 7, 3) == true)
        {
			//Execute node
			node_1_7();
			finish_node(current_job, 7, 3);
		}
		else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
        }
        
        old_job = current_job;
    }

}

void *sequence_1_4_function(void *arguments)
{
    set_cpu(3);

    struct job_token *old_job = NULL;
    struct job_token *queue_start = &task_1_jobs_queue[0];
    struct job_token *current_job = NULL;
    bool *precedence_matrix = task_1_precedence_constraints;

    while(true)
    {

        //Get pointer to the new job
        current_job = GetNewJob(current_job, queue_start, Q_SIZE, &task_1_sequence_4_semaphore);
        
        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 6, precedence_matrix, 7, 4) == true)
        {
			//Execute node
			node_1_6();
			finish_node(current_job, 6, 4);
		}
		else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
        }

        //Try to claim node. Go on if succesfull and terminate sequence if not.
        if(claim_node(current_job, 7, precedence_matrix, 7, 4) == true)
        {
			//Execute node
			node_1_7();
			finish_node(current_job, 7, 4);
		}
		else
        {
            //Terminate sequence
            old_job = current_job;
            continue;
        }
        
        old_job = current_job;
        
       
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
