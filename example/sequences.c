#include "sequences.h"
//semaphores
sem_t semaphores_T1[5];
sem_t semaphores_T2[6];
sem_t semaphores_T3[7];
sem_t semaphores_T4[5];
//threads
pthread_t task1_threads[5];
pthread_t task2_threads[6];
pthread_t task3_threads[7];
pthread_t task4_threads[5];

//sequence functions pointers
void *(*seq_func_ptr_t1[5])() = {sequence_1_1_function, sequence_1_2_function, sequence_1_3_function, sequence_1_4_function, sequence_1_5_function};
void *(*seq_func_ptr_t2[6])() = {sequence_2_1_function, sequence_2_2_function, sequence_2_3_function, sequence_2_4_function, sequence_2_5_function, sequence_2_6_function};
void *(*seq_func_ptr_t3[7])() = {sequence_3_1_function, sequence_3_2_function, sequence_3_3_function, sequence_3_4_function, sequence_3_5_function, sequence_3_6_function, sequence_3_7_function};
void *(*seq_func_ptr_t4[5])() = {sequence_4_1_function, sequence_4_2_function, sequence_4_3_function, sequence_4_4_function, sequence_4_5_function};

//node functions pointers
void (*nodes_func_ptr_t1[11])() = {node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7, node_1_8, node_1_9, node_1_10, node_1_11};
void (*nodes_func_ptr_t2[12])() = {node_2_1, node_2_2, node_2_3, node_2_4, node_2_5, node_2_6, node_2_7, node_2_8, node_2_9, node_2_10, node_2_11, node_2_12};
void (*nodes_func_ptr_t3[13])() = {node_3_1, node_3_2, node_3_3, node_3_4, node_3_5, node_3_6, node_3_7, node_3_8, node_3_9, node_3_10, node_3_11, node_3_12, node_3_13};
void (*nodes_func_ptr_t4[9])() = {node_4_1, node_4_2, node_4_3, node_4_4, node_4_5, node_4_6, node_4_7, node_4_8, node_4_9};

void *(* seq_func_ptr[23])() = {sequence_1_1_function, sequence_1_2_function, sequence_1_3_function, sequence_1_4_function, sequence_1_5_function, sequence_2_1_function, sequence_2_2_function, sequence_2_3_function, sequence_2_4_function, sequence_2_5_function, sequence_2_6_function, sequence_3_1_function, sequence_3_2_function, sequence_3_3_function, sequence_3_4_function, sequence_3_5_function, sequence_3_6_function, sequence_3_7_function, sequence_4_1_function, sequence_4_2_function, sequence_4_3_function, sequence_4_4_function, sequence_4_5_function};

//precedence constraints
uint64_t T1_precedence_constraints_h[11] = {0, 1, 2, 2, 2, 28, 1, 64, 64, 384, 544};
uint64_t T1_precedence_constraints_v[11] = {66, 28, 32, 32, 32, 1024, 384, 512, 512, 1024, 0};
uint64_t T2_precedence_constraints_h[12] = {0, 1, 2, 2, 12, 1, 32, 32, 32, 32, 960, 1040};
uint64_t T2_precedence_constraints_v[12] = {34, 12, 16, 16, 2048, 960, 1024, 1024, 1024, 1024, 2048, 0};
uint64_t T3_precedence_constraints_h[13] = {0, 1, 2, 2, 12, 1, 32, 32, 32, 448, 1, 1, 3600};
uint64_t T3_precedence_constraints_v[13] = {3106, 12, 16, 16, 4096, 448, 512, 512, 512, 4096, 4096, 4096, 0};
uint64_t T4_precedence_constraints_h[9] = {0, 1, 1, 4, 4, 4, 4, 120, 130};
uint64_t T4_precedence_constraints_v[9] = {6, 256, 120, 128, 128, 128, 128, 256, 0};

//sequences heads
uint64_t T1_sequence_heads[11] = {1, 0, 0, 5, 4, 0, 2, 0, 3, 0, 0};
uint64_t T2_sequence_heads[12] = {1, 0, 0, 6, 0, 2, 0, 5, 4, 3, 0, 0};
uint64_t T3_sequence_heads[13] = {1, 0, 0, 7, 0, 4, 0, 6, 5, 0, 3, 2, 0};
uint64_t T4_sequence_heads[9] = {1, 0, 2, 0, 5, 4, 3, 0, 0};

//task data structures
struct task_data task1_data = { .task_id =1, .priority =97, .number_of_nodes =11, .number_of_sequences = 5, 
.job_counter = 0, .pre_cons_h = T1_precedence_constraints_h, .pre_cons_v = T1_precedence_constraints_v, 
.sequence_heads = T1_sequence_heads, .sequences_guards = semaphores_T1, .last_added_job = NULL, .seq_threads = task1_threads, .period = 26586, 
.func = {node_1_1, node_1_2, node_1_3, node_1_4, node_1_5, node_1_6, node_1_7, node_1_8, node_1_9, node_1_10, node_1_11}}; 
struct task_data task2_data = { .task_id =2, .priority =95, .number_of_nodes =12, .number_of_sequences = 6, 
.job_counter = 0, .pre_cons_h = T2_precedence_constraints_h, .pre_cons_v = T2_precedence_constraints_v, 
.sequence_heads = T2_sequence_heads, .sequences_guards = semaphores_T2, .last_added_job = NULL, .seq_threads = task2_threads, .period = 33768, 
.func = {node_2_1, node_2_2, node_2_3, node_2_4, node_2_5, node_2_6, node_2_7, node_2_8, node_2_9, node_2_10, node_2_11, node_2_12}}; 
struct task_data task3_data = { .task_id =3, .priority =98, .number_of_nodes =13, .number_of_sequences = 7, 
.job_counter = 0, .pre_cons_h = T3_precedence_constraints_h, .pre_cons_v = T3_precedence_constraints_v, 
.sequence_heads = T3_sequence_heads, .sequences_guards = semaphores_T3, .last_added_job = NULL, .seq_threads = task3_threads, .period = 26124, 
.func = {node_3_1, node_3_2, node_3_3, node_3_4, node_3_5, node_3_6, node_3_7, node_3_8, node_3_9, node_3_10, node_3_11, node_3_12, node_3_13}}; 
struct task_data task4_data = { .task_id =4, .priority =96, .number_of_nodes =9, .number_of_sequences = 5, 
.job_counter = 0, .pre_cons_h = T4_precedence_constraints_h, .pre_cons_v = T4_precedence_constraints_v, 
.sequence_heads = T4_sequence_heads, .sequences_guards = semaphores_T4, .last_added_job = NULL, .seq_threads = task4_threads, .period = 32508, 
.func = {node_4_1, node_4_2, node_4_3, node_4_4, node_4_5, node_4_6, node_4_7, node_4_8, node_4_9}}; 

struct task_data *tasks_data[20] = {&task1_data, &task2_data, &task3_data, &task4_data};

//sequences functions
void *sequence_1_1_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(RBS_Execute(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(RBS_Execute(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
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
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,7) != 0)
   {
     TerminateSequence(seq_data, 7);
     continue;
   }

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_3_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_4_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_1_5_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_1_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(RBS_Execute(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(RBS_Execute(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_2_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,7) != 0)
   {
     TerminateSequence(seq_data, 7);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_3_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_4_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_5_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_2_6_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_1_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(RBS_Execute(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(RBS_Execute(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_2_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,12) != 0)
   {
     TerminateSequence(seq_data, 12);
     continue;
   }

   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_3_function(void *arguments)
{
set_cpu(2);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,11) != 0)
   {
     TerminateSequence(seq_data, 11);
     continue;
   }

   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_4_function(void *arguments)
{
set_cpu(1);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,7) != 0)
   {
     TerminateSequence(seq_data, 7);
     continue;
   }

   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_5_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_6_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,10) != 0)
   {
     TerminateSequence(seq_data, 10);
     continue;
   }

   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_3_7_function(void *arguments)
{
set_cpu(0);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,13) != 0)
   {
     TerminateSequence(seq_data, 13);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_1_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,1) != 0)
   {
     TerminateSequence(seq_data, 1);
     continue;
   }

   if(RBS_Execute(seq_data,2) != 0)
   {
     TerminateSequence(seq_data, 2);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_2_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,3) != 0)
   {
     TerminateSequence(seq_data, 3);
     continue;
   }

   if(RBS_Execute(seq_data,4) != 0)
   {
     TerminateSequence(seq_data, 4);
     continue;
   }

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_3_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,7) != 0)
   {
     TerminateSequence(seq_data, 7);
     continue;
   }

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_4_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,6) != 0)
   {
     TerminateSequence(seq_data, 6);
     continue;
   }

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   FinishJob(seq_data);
  }
}
void *sequence_4_5_function(void *arguments)
{
set_cpu(3);
 struct sequence_data *seq_data = (struct sequence_data*) arguments;
 while(true)
  {
   RBS_Wait(seq_data);

   if(RBS_Execute(seq_data,5) != 0)
   {
     TerminateSequence(seq_data, 5);
     continue;
   }

   if(RBS_Execute(seq_data,8) != 0)
   {
     TerminateSequence(seq_data, 8);
     continue;
   }

   if(RBS_Execute(seq_data,9) != 0)
   {
     TerminateSequence(seq_data, 9);
     continue;
   }

   FinishJob(seq_data);
  }
}
