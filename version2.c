#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>


#define CORES     2
#define NUM_ND_T1 7
#define NUM_ND_T2 5

#define NUM_SQ_T1 4
#define NUM_SQ_T2 3

pthread_mutex_t t1_locks[NUM_ND_T1];
pthread_mutex_t t2_locks[NUM_ND_T2];


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


void *node_function(void *arguments)
{
  struct node_info *info = arguments;
  int task_index = 1;
  int node_index = 1;
  int index = 0;
  printf("NODE %d,%d initialized.\n", info->task_number, info->node_number);


  if(info->task_number == 1)
  {
    pthread_mutex_lock(&t1_locks[info->node_number-1]);
  }
  else
  {
    pthread_mutex_lock(&t2_locks[info->node_number-1]);
  }

  printf("NODE %d,%d Started executing.\n", info->task_number, info->node_number);

  while(index != info->execution_time)
  {
    for (int i = 0; i < (0xFFFFFFF); i++);
    index ++;
  }
  printf("NODE 1,1 Ended.\n");

  if(info->task_number == 1)
  {
    pthread_mutex_unlock(&t1_locks[info->node_number-1]);
  }
  else
  {
    pthread_mutex_unlock(&t2_locks[info->node_number-1]);
  }

  pthread_exit((void *) 0);
  return NULL;
}

void create_binary_semaphores()
{
  for(int i = 0; i < NUM_ND_T1; i++)
  {
    pthread_mutex_init(&t1_locks[i], NULL);
    pthread_mutex_lock(&t1_locks[i]);
  }
    for(int i = 0; i < NUM_ND_T2; i++)
  {
    pthread_mutex_init(&t2_locks[i], NULL);
    pthread_mutex_lock(&t2_locks[i]);
  }

}

void destroy_binary_semaphores()
{
  for(int i = 0; i < NUM_ND_T1; i++)
  {
    pthread_mutex_destroy(&t1_locks[i]);
  }
    for(int i = 0; i < NUM_ND_T2; i++)
  {
    pthread_mutex_destroy(&t2_locks[i]);
  }
}




int main(void) {

  create_binary_semaphores();
  
  struct node_info t1_nodes[NUM_ND_T1];
  struct node_info t2_nodes[NUM_ND_T2];

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

  pthread_t task1_threads[NUM_ND_T1];
  pthread_t task2_threads[NUM_ND_T2];

  for(int i = 0; i < NUM_ND_T1; i++)
  {
    int result = pthread_create(&task1_threads[i], NULL, &node_function, (void *)&t1_nodes[i]);
  }

  for(int i = 0; i < NUM_ND_T2; i++)
  {
    int result = pthread_create(&task2_threads[i], NULL, &node_function, (void *)&t2_nodes[i]);
  }



  //pthread_mutex_unlock(&t1_lock);

  for (int i = 0; i < (0xFFFFFFF); i++);


  destroy_binary_semaphores();
  printf("main function ends\n");  
  }