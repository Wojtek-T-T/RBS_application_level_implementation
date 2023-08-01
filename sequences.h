#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "rbs_lib.h"
#include "workload.h"

#define NR_NODES_T1 7
#define NR_NODES_T2 5

#define NR_SEQ_T1 4
#define NR_SEQ_T2 3


void *(*seq_func_ptr_t1[NR_SEQ_T1])();

void (*nodes_func_ptr_t1[NR_NODES_T1])();


void *(*seq_func_ptr_t2[NR_SEQ_T2])();

void (*nodes_func_ptr_t2[NR_NODES_T2])();


bool task_1_precedence_constraints[36];
bool task_2_precedence_constraints[16];

int task_1_sequence_heads[NR_SEQ_T1-1];
int task_2_sequence_heads[NR_SEQ_T2-1];


//semaphores
sem_t semaphores_T1[NR_SEQ_T1];
sem_t semaphores_T2[NR_SEQ_T2];


//Threads
pthread_t task1_threads[NR_SEQ_T1];
pthread_t task2_threads[NR_SEQ_T2];

pthread_t task_1_sequence_1_thread;
pthread_t task_1_sequence_2_thread;
pthread_t task_1_sequence_3_thread;
pthread_t task_1_sequence_4_thread;

pthread_t task_2_sequence_1_thread;
pthread_t task_2_sequence_2_thread;
pthread_t task_2_sequence_3_thread;



//Sequence functions
void *sequence_1_1_function(void *arguments);
void *sequence_1_2_function(void *arguments);
void *sequence_1_3_function(void *arguments);
void *sequence_1_4_function(void *arguments);
void *sequence_2_1_function(void *arguments);
void *sequence_2_2_function(void *arguments);
void *sequence_2_3_function(void *arguments);


#endif
