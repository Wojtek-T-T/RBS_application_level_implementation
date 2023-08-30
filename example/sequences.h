#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "rbs_lib.h"
#include "workload.h"

#define NR_NODES_T1 7
#define NR_NODES_T2 5

#define NR_SEQ_T1 4
#define NR_SEQ_T2 3

//Tasks data structures
struct task_data task1_data;
struct task_data task2_data;

//Task 1 sequence and node functions
void *(*seq_func_ptr_t1[NR_SEQ_T1])();
void (*nodes_func_ptr_t1[NR_NODES_T1])();


//Task 2 sequence and node functions
void *(*seq_func_ptr_t2[NR_SEQ_T2])();
void (*nodes_func_ptr_t2[NR_NODES_T2])();

//Precedence constraints
//bool task_1_precedence_constraints[36];
//bool task_2_precedence_constraints[16];

u_int32_t T1_precedence_constraints_h[NR_NODES_T1];
u_int32_t T1_precedence_constraints_v[NR_NODES_T1];

u_int32_t T2_precedence_constraints_h[NR_NODES_T2];
u_int32_t T2_precedence_constraints_v[NR_NODES_T2];

//Sequence heads
u_int32_t T1_sequence_heads[NR_NODES_T1];
u_int32_t T2_sequence_heads[NR_NODES_T2];


//semaphores
sem_t semaphores_T1[NR_SEQ_T1];
sem_t semaphores_T2[NR_SEQ_T2];


//Threads
pthread_t task1_threads[NR_SEQ_T1];
pthread_t task2_threads[NR_SEQ_T2];

//Sequence functions
void *sequence_1_1_function(void *arguments);
void *sequence_1_2_function(void *arguments);
void *sequence_1_3_function(void *arguments);
void *sequence_1_4_function(void *arguments);
void *sequence_2_1_function(void *arguments);
void *sequence_2_2_function(void *arguments);
void *sequence_2_3_function(void *arguments);

#endif
