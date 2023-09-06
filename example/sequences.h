#ifndef SEQUENCES_H
#define SEQUENCES_H
#include "rbs_lib.h"
#include "workload.h"

#define number_of_tasks 2

//tasks data structures
struct task_data task1_data;
struct task_data task2_data;


struct task_data *tasks_data[20];

//sequence functions pointers
void *(*seq_func_ptr_t1[4])();
void *(*seq_func_ptr_t2[3])();
void *(*seq_func_ptr[7])();

//node functions pointers
void (*nodes_func_ptr_t1[7])();
void (*nodes_func_ptr_t2[5])();

//horizontal and vertical precedence constraints
u_int32_t T1_precedence_constraints_h[7];
u_int32_t T1_precedence_constraints_v[7];
u_int32_t T2_precedence_constraints_h[5];
u_int32_t T2_precedence_constraints_v[5];

//arrays with sequence heads
u_int32_t T1_sequence_heads[7];
u_int32_t T2_sequence_heads[5];

//semaphores
sem_t semaphores_T1[4];
sem_t semaphores_T2[3];

//threads
pthread_t task1_threads[4];
pthread_t task2_threads[3];

//sequence functions prototypes
void *sequence_1_1_function(void *arguments);
void *sequence_1_2_function(void *arguments);
void *sequence_1_3_function(void *arguments);
void *sequence_1_4_function(void *arguments);
void *sequence_2_1_function(void *arguments);
void *sequence_2_2_function(void *arguments);
void *sequence_2_3_function(void *arguments);

#endif