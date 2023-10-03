#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include "sequences.h"


struct job_timer_data
{
    int task_number;
    int period_in_usec;
    int max_number_of_jobs;
};

pthread_t job_release_threads[20];
void *job_release_func(void *arguments);

int main(void) 
{    

    int result = 0;

    //Initialize RBS
    initialize_rbs();

    //Initialize tasks and sequences
    int index = 0;
    for(int i = 0; i < number_of_tasks; i++)
    {
        InitializeTask(tasks_data[i]);

        
        int num_of_seq = tasks_data[i]->number_of_sequences;

        for(int x = 0 ; x < num_of_seq; x++)
        {

            pthread_t *t_ptr = (tasks_data[i]->seq_threads) + x;
            InitializeSequence(tasks_data[i], (x+1), t_ptr, tasks_data[i]->attr, seq_func_ptr[(index+x)]);

        } 
        index = index + num_of_seq;

    }

    pthread_attr_t attr;
	struct sched_param schedPARAM;

    pthread_attr_init(&attr);
    result = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    schedPARAM.sched_priority = 99;
    result = pthread_attr_setschedparam(&attr, &schedPARAM);
    result = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    for(int i = 3; i < number_of_tasks; i++)
    {
        struct job_timer_data *job_rel_tim = malloc(sizeof(struct job_timer_data));
        job_rel_tim->task_number = i;
        job_rel_tim->period_in_usec = tasks_data[i]->period;
        job_rel_tim->max_number_of_jobs = 300;
        result = pthread_create(&job_release_threads[i], &attr, &job_release_func, (void*) job_rel_tim);

    }
    
    //Wait for tasks to finish
    for(int i = 0; i < number_of_tasks; i++)
    {
        pthread_join(job_release_threads[i], NULL);
    }


    //print_log_data_json();
    //print_log_data_txt();


    print_log_data_json2(tasks_data[0], number_of_tasks);


    //CLose log
    closelog();

}


void *job_release_func(void *arguments)
{
    struct timespec deadline;
    int result = 0;
    int number_of_releases = 0;
    struct job_timer_data *tim_data = (struct job_timer_data*) arguments;

    int nano_seconds = tim_data->period_in_usec * 1000;

    
    while(true)
    {
        
        clock_gettime(CLOCK_REALTIME, &deadline);

        deadline.tv_nsec += nano_seconds;
        if(deadline.tv_nsec >= 1000000000)
        {
            deadline.tv_nsec -=1000000000;
            deadline.tv_sec++;
        }

        result = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &deadline, NULL);
    
        if(number_of_releases >= tim_data->max_number_of_jobs)
        {
            return 0;
        }
        else
        {
            ReleaseNewJob(tasks_data[tim_data->task_number]);
        }

        number_of_releases ++;

        
    }

}

