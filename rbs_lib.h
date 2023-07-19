#ifndef RBS_LIB_H
#define RBS_LIB_H


#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <syslog.h>
#include <time.h>
#include <sched.h>
#include <syslog.h>


#define NODE_EXECUTION_STARTED 0 
#define NEW_JOB_RELEASED 1
#define NODE_EXECUTION_FINISHED 2
#define JOB_EXECUTION_FINISHED 3

//task data structures
struct job_token
{
    u_int32_t job_execution_state;
    u_int32_t job_state;
    pthread_mutex_t job_lock;
    int task_id;
    int job_id;
};


void initialize_rbs();
void initialize_task(int number_of_nodes, int number_of_sequences, sem_t *semaphores, pthread_t *threads);
void log_info(int task, int sequence, int node, int job, clock_t time_stamp, int event);
void set_cpu(int cpu_num);
struct job_token* GetNewJob(struct job_token *old_job_ptr, struct job_token *start_ptr, int queue_size, sem_t *semaphore);
void initialize_new_job(struct job_token *last_added_job_ptr, struct job_token *queue_start_ptr, int queue_size, int task_id);
void finish_job(struct job_token *finished_job);
bool check_precedence_constraints(u_int8_t number_of_nodes, bool *precedence_matrix_pointer, u_int8_t node_number, struct job_token *job_pointer);
void finish_node(struct job_token *job, int finished_node, int sequence_id);
bool claim_node(struct job_token *job, int claimed_node, bool *precedence_matrix_pointer, u_int8_t number_of_nodes, int seq);
void signal_sequence_head(int node_to_signal, struct job_token *handled_job, sem_t *semaphore, int num_nodes, bool *precedence_matrix_pointer);
bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer);


#endif
