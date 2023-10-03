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
#include <stddef.h>

#define LOG_DATA
#define AUTO_SIGNAL


//LOG codes
#define NODE_EXECUTION 1 
#define NEW_JOB_RELEASED 2
#define JOB_EXECUTION_FINISHED 3
#define NODE_EXECUTION_FINISHED 0
#define SEQUENCE_TERMINATED 4
#define SIGNALLING_EXECUTION 5

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
	int period;
	int number_of_nodes;
	int number_of_sequences;
	int job_counter;
	u_int32_t *pre_cons_h;
	u_int32_t *pre_cons_v;
	u_int32_t *sequence_heads;
	sem_t *sequences_guards;
	struct job_token *last_added_job;
	pthread_attr_t attr;
	struct sched_param schedPARAM;
	void (*func[100])();

	pthread_t *seq_threads;
};


#ifdef LOG_DATA
struct log_event_data
{
	int task;
	int sequence;
	int node;
	int job;
	int event;
	struct timespec start_time;
	struct timespec end_time;
};

struct timespec time_reference;

pthread_mutex_t log_lock;
pthread_mutex_t release_lock;
struct log_event_data log_events_buffer[90000];
int log_events_counter;

struct log_event_data *log_event_buffers_ptrs[400];
int buff_indexes[400];
#endif
	
	

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


/*
* InitializeTask
* DESCRIPTION: This function initializes a task by setting its properties, initializing a double linked list and
			   and setting its priority and scheduling policy
* PARAMETERS: 
			   struct task_data *taskDATA = pointer to task_data structure containgng all info about the initialized task

* RETURN TYPE: INT: 
					0 = initialization was succesfull
					1 = Initialization failed due to priority of task ou of bounds
*/
int InitializeTask(struct task_data *taskDATA);



/*
* log_info
* DESCRIPTION: This function saves an event the the log buffer
* PARAMETERS: 
			   int task = task ID
			   int sequence = sequence ID
			   int node = node ID
			   int job = job ID
			   int event = event type (NODE_EXECUTION_STARTED, NEW_JOB_RELEASED, NODE_EXECUTION_FINISHED, JOB_EXECUTION_FINISHED, SEQUENCE_TERMINATED)
			   
* RETURN TYPE: 
				int event_index = index where the log event has been saved, needed for eventual addition of end time with log_event_end()
*/
struct log_event_data *log_event_start(int task, int sequence, int node, int job, int event);


/*
* log_event_end
* DESCRIPTION: This function adds the end time to an existing event in the log
* PARAMETERS: 
			   int event_index = Index where the already existing event is saved

* RETURN TYPE: NONE
*/
void log_event_end(struct log_event_data *ptr);


/*
* set_cpu
* DESCRIPTION: This function maps a sequence thread to one specific CPU
* PARAMETERS: 
			   int cpu_num = CPU ID, the ID's are starting from 0 for core 1

* RETURN TYPE: NONE
*/
void set_cpu(int cpu_num);



/*
* WaitNextJob
* DESCRIPTION: This function blocks the execution of the thread of a sequence untill both: a new job has been released and all 
			   precedence of the head node of the sequence have been fulfilled

* PARAMETERS: 
			   struct sequence_data *sequenceDATA = pointer to sequence_data structure containgng all info about the sequence 
			   										from which the function is being called

* RETURN TYPE: NONE
*/
void WaitNextJob(struct sequence_data *sequenceDATA);


/*
* ReleaseNewJob
* DESCRIPTION: This function adds a new job of the provided task to the list. Internally function posts the primary guards of sequences.

* PARAMETERS: 
			   struct task_data *taskDATA = pointer to task_data structure containgng all info about the task

* RETURN TYPE: NONE
*/
void ReleaseNewJob(struct task_data *taskDATA);

void FinishJob(struct sequence_data *sequenceDATA);

bool check_precedence_constraints(struct sequence_data *sequenceDATA, u_int32_t node_number);

void MarkNodeExecuted(struct sequence_data *sequenceDATA, int finished_node);



/*
* TryExecuteNode
* DESCRIPTION: This function will try to claim and execute the provided node. If the node is already executed/in execution or 
			   can not be executed yet because of missing precedance constraints, the function will terminate.

* PARAMETERS: 
			   struct task_data *taskDATA = pointer to task_data structure containgng all info about the task

* RETURN TYPE: INT:
					0 = Node was executed sucessfully
					1 = Node could not be executed due to not fullfilled precedence constraints
					2 = Node could not be executed because it already has been executed or is being executed
						as a part of other sequence
*/
int TryExecuteNode(struct sequence_data *sequenceDATA, int node);

void SignalSequenceMan(struct sequence_data *sequenceDATA, int node_to_signal, sem_t *semaphore);

void SignalSequenceAut(int finished_node, struct sequence_data *sequenceDATA);

bool check_if_node_in_execution(u_int8_t node_number, struct job_token *job_pointer);

void TerminateSequence(struct sequence_data *sequenceDATA, int node);

void MarkNodeInExecution(struct sequence_data *sequenceDATA, int node);

void display_log_data();

void print_log_data_txt();

void print_log_data_json();

void print_log_data_json2(struct task_data *taskDATA_start, int num_of_tasks);


#endif