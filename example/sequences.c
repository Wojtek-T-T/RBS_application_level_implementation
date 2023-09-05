#include "sequences.h"

//task data structures
struct task_data task1_data = { .task_id =1, .priority =50, .number_of_nodes =7, .number_of_sequences = 4, 
.job_counter = 0, .pre_cons_h = T1_precedence_constraints_h, .pre_cons_v = T1_precedence_constraints_v, 
.sequence_heads = T1_sequence_heads, .sequences_guards = semaphores_T1, .last_added_job = NULL, 
.func = { node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7}}; 
struct task_data task2_data = { .task_id =2, .priority =10, .number_of_nodes =5, .number_of_sequences = 3, 
.job_counter = 0, .pre_cons_h = T2_precedence_constraints_h, .pre_cons_v = T2_precedence_constraints_v, 
.sequence_heads = T2_sequence_heads, .sequences_guards = semaphores_T2, .last_added_job = NULL, 
.func = { node_2_1, node_2_2, node_2_3, node_2_4, node_2_5}}; 


//sequence functions pointers
void *(*seq_func_ptr_t1[4])() = {sequence_1_1_function, sequence_1_2_function, sequence_1_3_function, sequence_1_4_function};
void *(*seq_func_ptr_t2[3])() = {sequence_2_1_function, sequence_2_2_function, sequence_2_3_function};

//node functions pointers
void (*nodes_func_ptr_t1[7])() = {node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7};
void (*nodes_func_ptr_t2[5])() = {node_2_1, node_2_2, node_2_3, node_2_4, node_2_5};

//precedence constraints
u_int32_t T1_precedence_constraints_h[7] = {0, 1, 1, 1, 14, 14, 48};
u_int32_t T1_precedence_constraints_v[7] = {14, 48, 48, 48, 64, 64, 0};
u_int32_t T2_precedence_constraints_h[5] = {0, 1, 1, 1, 14};
u_int32_t T2_precedence_constraints_v[5] = {14, 16, 16, 16, 0};

//sequences heads
u_int32_t T1_sequence_heads[7] = {1, 0, 2, 3, 0, 4, 0};
u_int32_t T2_sequence_heads[5] = {1, 0, 2, 3, 0};

//sequences functions
void *sequence_1_1_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {


   WaitNextJob(seq_data);

   if(TryExecuteNode(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(TryExecuteNode(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(TryExecuteNode(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(TryExecuteNode(seq_data,7) != 0)
   {
     TerminateSequence(seq_data, 7);
     continue;
   }

   FinishJob(seq_data);
  }
}


void *sequence_1_2_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {

   WaitNextJob(seq_data);

   if(TryExecuteNode(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(TryExecuteNode(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(TryExecuteNode(seq_data,7) != 0)
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
   WaitNextJob(seq_data);

   if(TryExecuteNode(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(TryExecuteNode(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(TryExecuteNode(seq_data,7) != 0)
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
   WaitNextJob(seq_data);

   if(TryExecuteNode(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(TryExecuteNode(seq_data,7) != 0)
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
   WaitNextJob(seq_data);

   if(TryExecuteNode(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(TryExecuteNode(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(TryExecuteNode(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
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
   WaitNextJob(seq_data);

   if(TryExecuteNode(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(TryExecuteNode(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
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
   WaitNextJob(seq_data);

   if(TryExecuteNode(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(TryExecuteNode(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   FinishJob(seq_data);
  }
}
