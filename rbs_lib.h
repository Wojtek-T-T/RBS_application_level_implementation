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


//LOG codes
#define NODE_EXECUTION_STARTED 0 
#define NEW_JOB_RELEASED 1
#define NODE_EXECUTION_FINISHED 2
#define JOB_EXECUTION_FINISHED 3

//task data structures
struct job_token
{
	struct job_token *next_job;
	struct job_token *previous_job;
	
    u_int32_t job_execution_state;
    u_int32_t job_state;
    pthread_mutex_t job_lock;
    int job_id;
};


//sequence data structure
struct sequence_data
{
	int sequence_id;
	int cpu_id;
	struct task_data *task;
	
	struct job_token *current_job;
	
	sem_t *semaphore;
	
};


struct task_data
{
	int task_id;
	int number_of_nodes;
	int job_counter;
	bool *precedence_matrix;
	sem_t *sem;
	struct job_token *last_added_job;
	
	void (*func[100])();
};
	
	


void InitializeSequence(struct task_data *taskDATA, int sequenceID, pthread_t *thread, sem_t *semaphore, void *(*func)());



void initialize_rbs();


/* This function initializes a task that is going to be executed according to RBS rules
 * PARAMETERS:
 * 		int taskID: ID number of the scheduled task 
 * 		bool *precedenceMATRIX: pointer to a data structure representing the precedence constraints between nodes of the task
 * 		int number_of_nodes: number of nodes of the task
 * 		sem_t *semaphores_address: pointer to the semaphore that is guarding the task 
 * 		void (*workload[])(): array of pointers to functions representing the workload of the nodes
 * RETURN TYPE:
 * 		struct task_data: pointer to the data structure containing the data of the initialized task
 */
struct task_data *InitializeTask(int taskID, bool *precedenceMATRIX, int number_of_nodes, sem_t *semaphores_address, void (*workload[])());



void log_info(int task, int sequence, int node, int job, clock_t time_stamp, int event);

void set_cpu(int cpu_num);

void WaitNextJob(struct sequence_data *sequenceDATA);

void ReleaseNewJob(struct task_data *taskDATA);

void finish_job(struct job_token *finished_job);

bool check_precedence_constraints(u_int8_t number_of_nodes, bool *precedence_matrix_pointer, u_int8_t node_number, struct job_token *job_pointer);

void finish_node(struct sequence_data *sequenceDATA, int finished_node);

int TryExecuteNode(struct sequence_data *sequenceDATA, int node);

void signal_sequence_head(int node_to_signal, struct job_token *handled_job, sem_t *semaphore, int num_nodes, bool *precedence_matrix_pointer);

bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer);


#endif
