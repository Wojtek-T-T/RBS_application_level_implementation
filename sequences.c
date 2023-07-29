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

void (*nodes_func_ptr_t1[NR_NODES_T1])() = {node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7 };


void *sequence_1_1_function(void *arguments)
{
    set_cpu(0);
    
    struct sequence_data *seq_data = (struct sequence_data*) arguments;
 
    while(true)
    {
		
		//Wait for release of a new job
		WaitNextJob(seq_data);
        
        
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 1) != 0)
        {
			continue;
        }

        
      
        //Signal other sequences
        signal_sequence_head(4, seq_data->current_job, &task_1_sequence_3_semaphore, 7, seq_data->task->precedence_matrix);
        signal_sequence_head(3, seq_data->current_job, &task_1_sequence_2_semaphore, 7, seq_data->task->precedence_matrix);
        
        
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 2) != 0)
        {
			continue;
        }


        //Signal other sequences
        signal_sequence_head(6, seq_data->current_job, &task_1_sequence_4_semaphore, 7, seq_data->task->precedence_matrix);


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
			continue;
        }
		

         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 7) != 0)
        {
			continue;
        }
		

    }
    

}

void *sequence_1_2_function(void *arguments)
{
	//Set CPU
    set_cpu(1);
    
    struct sequence_data *seq_data = (struct sequence_data*) arguments;

    while(true)
    {
		//Wait for release of a new job
		WaitNextJob(seq_data);
        
       
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 3) != 0)
        {
			continue;
        }
        

        //Signal other sequences
        signal_sequence_head(6, seq_data->current_job, &task_1_sequence_4_semaphore, 7, seq_data->task->precedence_matrix);

        
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
			continue;
        }


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 7) != 0)
        {
			continue;
        }

    }
    
}

void *sequence_1_3_function(void *arguments)
{
    set_cpu(2);
    
    struct sequence_data *seq_data = (struct sequence_data*) arguments;


    while(true)
    {

		//Wait for release of a new job
		WaitNextJob(seq_data);
        
        
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 4) != 0)
        {
			continue;
        }
        

        //Signal other sequences
        signal_sequence_head(6, seq_data->current_job, &task_1_sequence_4_semaphore, 7, seq_data->task->precedence_matrix);

         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
			continue;
        }


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 7) != 0)
        {
			continue;
        }
        
    }

}

void *sequence_1_4_function(void *arguments)
{
    set_cpu(3);
    
    struct sequence_data *seq_data = (struct sequence_data*) arguments;

    while(true)
    {

		//Wait for release of a new job
		WaitNextJob(seq_data);
        
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 6) != 0)
        {
			continue;
        }

         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 7) != 0)
        {
			continue;
        }      
       
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
