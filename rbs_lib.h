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

//job data structures
struct job_token
{
	//pointers to next and previous job tokens (needed for correct working of double linked list)
	struct job_token *next_job;
	struct job_token *previous_job;
	
	//Bitmap containg information which nodes of the job are being executed
    u_int32_t nodes_in_execution;

	//Bitmap containing information which nodes of the job are finished
    u_int32_t nodes_finished;

	//Lock that locks the job token
    pthread_mutex_t job_lock;

	//job ID (coresponding to the number of the release)
    int job_id;

	//pointer to secondary guarding semaphore
	sem_t *secondary_sequences_guards;
};


//sequence data structure
struct sequence_data
{
	//sequence ID
	int sequence_id;

	//cpu on which sequence should run
	int cpu_id;

	//pointer to the task structure (of which the sequence is being part of)
	struct task_data *task;

	//pointer to the token of the currently handled job
	struct job_token *current_job;

	//pointer to primary sequence guarding semaphores
	sem_t *semaphore;
	
};

//task data structure
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

struct log_event_data
{
	int task;
	int sequence;
	int node;
	int job;
	clock_t time_stamp;
	int event;

	struct timespec tim;

};

struct timespec time_reference;

pthread_mutex_t log_lock;
struct log_event_data log_events_buffer[10000];
int log_events_counter;
bool log_displayd_flag;
	
	

/*
* InitializeSequence
* DESCRIPTION: This function initializes a sequence of a task.
* PARAMETERS: 
			  struct task_data *taskDATA = pointer to the structure containg data of the task of which the initialized sequence is part of
*             int sequenceID = ID number of the sequence (within the task)
*			  pthread_t *thread = pointer to the thread wich will run the sequence function
*			  pthread_attr_t attr = structure containg atributes for the thread (needed for correct setting of scheduling policy and priority)
*			  void *(*func)() = pointer to the sequence function		
* RETURN TYPE: NONE
*/
void InitializeSequence(struct task_data *taskDATA, int sequenceID, pthread_t *thread, pthread_attr_t attr, void *(*func)());




/*
* InitializeRBS
* DESCRIPTION: This function set the scheduling policy of the main thread to SCHED_FIFO and its priority to 99 (highest priority)
* 			   as this thread is responsible for releasing of jobs.
* PARAMETERS: NONE
* RETURN TYPE: NONE
*/
void initialize_rbs();

int InitializeTask(struct task_data *taskDATA);

void log_info(int task, int sequence, int node, int job, int event);

void set_cpu(int cpu_num);

void WaitNextJob(struct sequence_data *sequenceDATA);

void ReleaseNewJob(struct task_data *taskDATA);

void FinishJob(struct sequence_data *sequenceDATA);

bool check_precedence_constraints(struct sequence_data *sequenceDATA, u_int8_t node_number);

void MarkNodeExecuted(struct sequence_data *sequenceDATA, int finished_node);

int TryExecuteNode(struct sequence_data *sequenceDATA, int node);

void SignalSequenceMan(struct sequence_data *sequenceDATA, int node_to_signal, sem_t *semaphore);

void SignalSequenceAut(int finished_node, struct sequence_data *sequenceDATA);

bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer);

void TerminateSequence(struct sequence_data *sequenceDATA, int node);

void MarkNodeInExecution(struct sequence_data *sequenceDATA, int node);

void display_log_data();


#endif
