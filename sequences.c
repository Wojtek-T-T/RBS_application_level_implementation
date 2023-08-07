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

int task_1_sequence_heads[NR_SEQ_T1-1] = {3, 4, 6};

int task_2_sequence_heads[NR_SEQ_T2-1] = {3, 4};


void *(*seq_func_ptr_t1[NR_SEQ_T1])() = {sequence_1_1_function, sequence_1_2_function, sequence_1_3_function, sequence_1_4_function};

void (*nodes_func_ptr_t1[NR_NODES_T1])() = {node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7 };


void *(*seq_func_ptr_t2[NR_SEQ_T2])() = {sequence_2_1_function, sequence_2_2_function, sequence_2_3_function};

void (*nodes_func_ptr_t2[NR_NODES_T2])() = {node_2_1, node_2_2, node_2_3, node_2_4, node_2_5 };


struct task_data task1_data = {.task_id = 1, .priority = 50, .number_of_nodes = 7, .number_of_sequences = 4, .job_counter = 0, 
.precedence_matrix = task_1_precedence_constraints, .sequence_heads = task_1_sequence_heads, .sequences_guards = semaphores_T1,
.last_added_job = NULL, .func = {node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7 }};

struct task_data task2_data = {.task_id = 2, .priority = 5, .number_of_nodes = 5, .number_of_sequences = 3, .job_counter = 0, 
.precedence_matrix = task_2_precedence_constraints, .sequence_heads = task_2_sequence_heads, .sequences_guards = semaphores_T2,
.last_added_job = NULL, .func = {node_2_1, node_2_2, node_2_3, node_2_4, node_2_5 }};


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
            TerminateSequence(seq_data, 1);
			continue;
        }
  
        
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 2) != 0)
        {
            TerminateSequence(seq_data, 2);
			continue;
        }


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
            TerminateSequence(seq_data, 5);
			continue;
        }
		

         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 7) != 0)
        {
            TerminateSequence(seq_data, 7);
			continue;
        }
		
		FinishJob(seq_data);

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
            TerminateSequence(seq_data, 3);
			continue;
        }
           
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
            TerminateSequence(seq_data, 5);
			continue;
        }


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 7) != 0)
        {
            TerminateSequence(seq_data, 7);
			continue;
        }
        
        FinishJob(seq_data);

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
            TerminateSequence(seq_data, 4);
			continue;
        }

         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
            TerminateSequence(seq_data, 5);
			continue;
        }


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 7) != 0)
        {
            TerminateSequence(seq_data, 7);
			continue;
        }
        
        FinishJob(seq_data);
        
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
            TerminateSequence(seq_data, 6);
			continue;
        }

         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 7) != 0)
        {
            TerminateSequence(seq_data, 7);
			continue;
        }
        
        FinishJob(seq_data);      
       
    }
}

void *sequence_2_1_function(void *arguments)
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
        
        
         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 2) != 0)
        {
			continue;
        }


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
			continue;
        }
        
        FinishJob(seq_data);		
       
    }
    

}

void *sequence_2_2_function(void *arguments)
{
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
       


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
			continue;
        }
        
        FinishJob(seq_data);	       
    }
}

void *sequence_2_3_function(void *arguments)
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
       


         //Try to execute node, terminate if precedence constraints not fullfield.
        if(TryExecuteNode(seq_data, 5) != 0)
        {
			continue;
        }
        
        FinishJob(seq_data);       
    }

}
