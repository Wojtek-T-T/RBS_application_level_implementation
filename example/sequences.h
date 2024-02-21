#ifndef SEQUENCES_H
#define SEQUENCES_H
#include "../rbs_lib_app/rbs_lib_app.h"
#include "workload.h"
#define number_of_tasks 4

//tasks data structures
extern struct task_data task1_data;
extern struct task_data task2_data;
extern struct task_data task3_data;
extern struct task_data task4_data;
extern struct task_data *tasks_data[20];

//sequence functions pointers
extern void *(*seq_func_ptr_t1[5])();
extern void *(*seq_func_ptr_t2[6])();
extern void *(*seq_func_ptr_t3[7])();
extern void *(*seq_func_ptr_t4[5])();

 extern void *(*seq_func_ptr[23])();

//node functions pointers
extern void (*nodes_func_ptr_t1[11])();
extern void (*nodes_func_ptr_t2[12])();
extern void (*nodes_func_ptr_t3[13])();
extern void (*nodes_func_ptr_t4[9])();

//horizontal and vertical precedence constraints
extern uint64_t T1_precedence_constraints_h[11];
extern uint64_t T1_precedence_constraints_v[11];
extern uint64_t T2_precedence_constraints_h[12];
extern uint64_t T2_precedence_constraints_v[12];
extern uint64_t T3_precedence_constraints_h[13];
extern uint64_t T3_precedence_constraints_v[13];
extern uint64_t T4_precedence_constraints_h[9];
extern uint64_t T4_precedence_constraints_v[9];

//arrays with sequence heads
extern uint64_t T1_sequence_heads[11];
extern uint64_t T2_sequence_heads[12];
extern uint64_t T3_sequence_heads[13];
extern uint64_t T4_sequence_heads[9];

//semaphores
extern sem_t semaphores_T1[5];
extern sem_t semaphores_T2[6];
extern sem_t semaphores_T3[7];
extern sem_t semaphores_T4[5];

//threads
extern pthread_t task1_threads[5];
extern pthread_t task2_threads[6];
extern pthread_t task3_threads[7];
extern pthread_t task4_threads[5];

//sequence functions prototypes
void *sequence_1_1_function(void *arguments);
void *sequence_1_2_function(void *arguments);
void *sequence_1_3_function(void *arguments);
void *sequence_1_4_function(void *arguments);
void *sequence_1_5_function(void *arguments);
void *sequence_2_1_function(void *arguments);
void *sequence_2_2_function(void *arguments);
void *sequence_2_3_function(void *arguments);
void *sequence_2_4_function(void *arguments);
void *sequence_2_5_function(void *arguments);
void *sequence_2_6_function(void *arguments);
void *sequence_3_1_function(void *arguments);
void *sequence_3_2_function(void *arguments);
void *sequence_3_3_function(void *arguments);
void *sequence_3_4_function(void *arguments);
void *sequence_3_5_function(void *arguments);
void *sequence_3_6_function(void *arguments);
void *sequence_3_7_function(void *arguments);
void *sequence_4_1_function(void *arguments);
void *sequence_4_2_function(void *arguments);
void *sequence_4_3_function(void *arguments);
void *sequence_4_4_function(void *arguments);
void *sequence_4_5_function(void *arguments);
#endif
