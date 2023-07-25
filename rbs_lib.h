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


//sequence data structure
struct sequence_data
{
	int task_id;
	int sequence_id;
	int cpu_id;
	int queue_size;
	int number_of_nodes;
	
	struct job_token *current_job;
	struct job_token *previous_job;
	struct job_token *queue_address;
	bool *precedence_matrix;
	
	sem_t *semaphore;
	
	void (*workload[100])();
	
};


struct task_data
{
	int task_id;
	int queue_size;
	int number_of_nodes;
	int job_counter;
	bool *precedence_matrix;
	struct job_token *queue_address;
	struct job_token *last_released_job;
	
	sem_t *sem;
	
	void (*func[100])();
};
	
	


void InitializeSequence(struct task_data *taskDATA, struct sequence_data *sequenceDATA, int sequenceID, pthread_t *thread, sem_t *semaphore, void *(*func)());

void initialize_rbs();

void InitializeTask(struct task_data *taskDATA, int taskID, int number_of_nodes, int queueSIZE, struct job_token *queueADDRESS, bool *precedenceMATRIX, sem_t *semaphores_address, void (*workload[])());

void log_info(int task, int sequence, int node, int job, clock_t time_stamp, int event);

void set_cpu(int cpu_num);

void WaitNextJob(struct sequence_data *sequenceDATA);

void ReleaseNewJob(struct task_data *taskDATA);

void finish_job(struct job_token *finished_job);

bool check_precedence_constraints(u_int8_t number_of_nodes, bool *precedence_matrix_pointer, u_int8_t node_number, struct job_token *job_pointer);

void finish_node(struct job_token *job, int finished_node, int sequence_id);

int TryExecuteNode(struct sequence_data *sequenceDATA, int node);

void signal_sequence_head(int node_to_signal, struct job_token *handled_job, sem_t *semaphore, int num_nodes, bool *precedence_matrix_pointer);

bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer);

void TerminateSequence(struct sequence_data *sequenceDATA);


#endif
