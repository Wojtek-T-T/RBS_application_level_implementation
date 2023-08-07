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

#define LOG_DATA
#define AUTO_SIGNAL


//LOG codes
#define NODE_EXECUTION_STARTED 0 
#define NEW_JOB_RELEASED 1
#define NODE_EXECUTION_FINISHED 2
#define JOB_EXECUTION_FINISHED 3
#define SEQUENCE_TERMINATED 4

//task data structures
struct job_token
{
	struct job_token *next_job;
	struct job_token *previous_job;
	
    u_int32_t job_execution_state;
    u_int32_t job_state;
    pthread_mutex_t job_lock;
    int job_id;

	sem_t *secondary_sequences_guards;
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
	int priority;
	int number_of_nodes;
	int number_of_sequences;
	int job_counter;
	bool *precedence_matrix;
	int *sequence_heads;
	sem_t *sequences_guards;
	struct job_token *last_added_job;
	pthread_attr_t attr;
	struct sched_param schedPARAM;
	
	void (*func[100])();
};
	
	


void InitializeSequence(struct task_data *taskDATA, int sequenceID, pthread_t *thread, pthread_attr_t attr, void *(*func)());



void initialize_rbs();


int InitializeTask(struct task_data *taskDATA);

void log_info(int task, int sequence, int node, int job, clock_t time_stamp, int event);

void set_cpu(int cpu_num);

void WaitNextJob(struct sequence_data *sequenceDATA);

void ReleaseNewJob(struct task_data *taskDATA);

void FinishJob(struct sequence_data *sequenceDATA);

bool check_precedence_constraints(struct sequence_data *sequenceDATA, u_int8_t node_number);

void finish_node(struct sequence_data *sequenceDATA, int finished_node);

int TryExecuteNode(struct sequence_data *sequenceDATA, int node);

void SignalSequenceMan(struct sequence_data *sequenceDATA, int node_to_signal, sem_t *semaphore);

void SignalSequenceAut(int finished_node, struct sequence_data *sequenceDATA);

bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer);

void TerminateSequence(struct sequence_data *sequenceDATA, int node);


#endif
