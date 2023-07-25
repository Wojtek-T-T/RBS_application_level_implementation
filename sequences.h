#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "rbs_lib.h"
#include "workload.h"

#define Q_SIZE 10

#define NR_NODES_T1 7
#define NR_NODES_T2 5

#define NR_SEQ_T1 4
#define NR_SEQ_T2 3


void *(*seq_func_ptr_t1[NR_SEQ_T1])();

void (*nodes_func_ptr_t1[NR_NODES_T1])();


bool task_1_precedence_constraints[36];
bool task_2_precedence_constraints[16];


//semaphores
sem_t semaphores_T1[NR_SEQ_T1];
sem_t semaphores_T2[NR_SEQ_T2];



sem_t task_1_semaphore;
sem_t task_1_sequence_2_semaphore;
sem_t task_1_sequence_3_semaphore;
sem_t task_1_sequence_4_semaphore;

sem_t task_2_semaphore;
sem_t task_2_sequence_2_semaphore;
sem_t task_2_sequence_3_semaphore;


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


//Tasks queues
struct job_token task_1_jobs_queue[Q_SIZE];
struct job_token task_2_jobs_queue[Q_SIZE];


//Sequence functions
void *sequence_1_1_function(void *arguments);
void *sequence_1_2_function(void *arguments);
void *sequence_1_3_function(void *arguments);
void *sequence_1_4_function(void *arguments);
void *sequence_2_1_function(void *arguments);
void *sequence_2_2_function(void *arguments);
void *sequence_2_3_function(void *arguments);


#endif
