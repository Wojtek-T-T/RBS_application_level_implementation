#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>


#define CORES     2
#define NUM_TASKS 2

#define NUM_ND_T1 7
#define NUM_ND_T2 5
#define NUM_ND 12
#define LONGEST_SEQ 7

#define NUM_SQ_T1 4
#define NUM_SQ_T2 3

#define LN_SQ_T1 4
#define LN_SQ_T2 3

pthread_mutex_t t_locks[(LONGEST_SEQ * NUM_TASKS)];

int node_status[(LONGEST_SEQ * NUM_TASKS)];


int task1_execution_times [NUM_ND_T1] = {1,2,3,1,2,3,2};
int task2_execution_times [NUM_ND_T2] = {2,2,2,2,2};

int tasks_priorities[2] = {1,2};

int precedence_matrix_t1 [NUM_ND_T1*NUM_ND_T1] = {0,0,0,0,0,0,0,
                                                  1,0,0,0,0,0,0,
                                                  1,0,0,0,0,0,0,
                                                  1,0,0,0,0,0,0,
                                                  0,1,1,1,0,0,0,
                                                  0,0,1,1,0,0,0,
                                                  0,0,0,0,1,1,0};

int precedence_matric_t2 [NUM_ND_T2*NUM_ND_T2] = {0,0,0,0,0,
                                                  1,0,0,0,0,
                                                  1,0,0,0,0,
                                                  1,0,0,0,0,
                                                  0,1,1,1,0};

int task1_sequences [16] = {1,2,5,7,
                            3,5,7,0,
                            4,5,7,0,
                            6,7,0,0};

 int task2_sequences [9] = {1,2,5,
                            3,5,0,
                            4,5,0};                          

struct node_info {
    int task_number;
    int node_number;
    int execution_time;
};

struct sequence_info {
    int task_number;
    int sequence_number;
    int number_of_nodes;
    struct node_info *nodes [10];
};


void *node_function(void *arguments)
{
  struct sequence_info *seq_info = arguments;

  printf("SEQUENCE %d,%d initialized.\n", seq_info->task_number, seq_info->sequence_number);

  //printf("SEQUENCE %d,%d, start node is %d\n", seq_info->task_number, seq_info->sequence_number, seq_info->nodes[0]->node_number);


  //walk through all nodes in the sequence
  for(int node_nr = 0; node_nr < seq_info->number_of_nodes; node_nr ++)
  {
    struct node_info *current_node = seq_info->nodes[node_nr];

    int index = ((current_node->task_number -1) *LONGEST_SEQ) + current_node->node_number - 1;

    pthread_mutex_lock(&t_locks[index]);
    
    //if node has not yet been executed execute it, otherwise do nothing
    if(node_status[index] == 0)
    {
      printf("SEQUENCE %d,%d, executes node %d\n", seq_info->task_number, seq_info->sequence_number, current_node->node_number);
      for(int a = 0; a < current_node->execution_time; a++)
      {
        for (int i = 0; i < (0xFFFFFFF); i++);
      }
      node_status[index] = 1;

    }
    else
    {

    }

    pthread_mutex_unlock(&t_locks[index]);
  }
   
  pthread_exit((void *) 0);
  return NULL;
}

void create_binary_semaphores()
{
  for(int i = 0; i < (LONGEST_SEQ * NUM_TASKS); i++)
  {
    pthread_mutex_init(&t_locks[i], NULL);
    pthread_mutex_lock(&t_locks[i]);
  }
}

void destroy_binary_semaphores()
{
  for(int i = 0; i < (LONGEST_SEQ * NUM_TASKS); i++)
  {
    pthread_mutex_destroy(&t_locks[i]);
  }
}




int main(void) {

  create_binary_semaphores();
  
  struct node_info t1_nodes[NUM_ND_T1];
  struct node_info t2_nodes[NUM_ND_T2];

  struct sequence_info t1_sequences[NUM_SQ_T1];
  struct sequence_info t2_sequences[NUM_SQ_T2];

  for(int i = 0; i < NUM_ND_T1; i++)
  {
    t1_nodes[i].task_number = 1;
    t1_nodes[i].node_number = i+1;
    t1_nodes[i].execution_time = task1_execution_times[i];
  }

  for(int i = 0; i < NUM_ND_T2; i++)
  {
    t2_nodes[i].task_number = 2;
    t2_nodes[i].node_number = i+1;
    t2_nodes[i].execution_time = task2_execution_times[i];
  }


  for(int w = 0; w < NUM_SQ_T1; w++)
  {
    t1_sequences[w].number_of_nodes = NUM_ND_T1;
    t1_sequences[w].sequence_number = w+1;
    t1_sequences[w].task_number = 1;

    for(int p = 0; p < LN_SQ_T1; p++)
    {
      int node_ind = (w * NUM_SQ_T1) + p;
      int node_num = task1_sequences[node_ind];
      t1_sequences[w].nodes[p] = &t1_nodes[node_num-1];
    }
  }

  for(int w = 0; w < NUM_SQ_T2; w++)
  {
    t2_sequences[w].number_of_nodes = NUM_ND_T2;
    t2_sequences[w].sequence_number = w+1;
    t2_sequences[w].task_number = 2;

    for(int p = 0; p < LN_SQ_T2; p++)
    {
      int node_ind = (w * NUM_SQ_T2) + p;
      int node_num = task2_sequences[node_ind];
      t2_sequences[w].nodes[p] = &t2_nodes[node_num-1];
    }
  }


  pthread_t task1_threads[NUM_SQ_T1];
  pthread_t task2_threads[NUM_SQ_T2];


  //Create threads for task 1
  for(int i = 0; i < NUM_SQ_T1; i++)
  {
    int result = pthread_create(&task1_threads[i], NULL, &node_function, (void *)&t1_sequences[i]);
  }

  //Create threads for task 2
  for(int i = 0; i < NUM_SQ_T2; i++)
  {
    int result = pthread_create(&task2_threads[i], NULL, &node_function, (void *)&t2_sequences[i]);
  }



  pthread_mutex_unlock(&t_locks[0]);

  for (int i = 0; i < (0xFFFFFFFF); i++);


  destroy_binary_semaphores();
  printf("main function ends\n");  
  }
