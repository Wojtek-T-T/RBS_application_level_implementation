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
void *job_release_func_bounded(void *arguments);
void *job_release_func(void *arguments);

int main(void) 
{    

    int result = 0;
    int task_with_biggest_period = 0;
    int biggest_period = 0;

    //Initialize RBS
    initialize_rbs();

    //Initialize tasks and sequences
    int index = 0;
    for(int i = 0; i < number_of_tasks; i++)
    {
        InitializeTask(tasks_data[i]);

        if(tasks_data[i]->period > biggest_period)
        {
            biggest_period = tasks_data[i]->period;
            task_with_biggest_period = i;
        }

        
        int num_of_seq = tasks_data[i]->number_of_sequences;

        for(int x = 0 ; x < num_of_seq; x++)
        {

            pthread_t *t_ptr = (tasks_data[i]->seq_threads) + x;
            InitializeSequence(tasks_data[i], (x+1), t_ptr, tasks_data[i]->attr, seq_func_ptr[(index+x)]);

        } 
        index = index + num_of_seq;

    }

    pthread_attr_t attr[20];
	struct sched_param schedPARAM[20];


    for(int i = 0; i < number_of_tasks; i++)
    {
        pthread_attr_init(&attr[i]);
        result = pthread_attr_setschedpolicy(&attr[i], SCHED_FIFO);
        schedPARAM[i].sched_priority = tasks_data[i]->priority;
        result = pthread_attr_setschedparam(&attr[i], &schedPARAM[i]);
        result = pthread_attr_setinheritsched(&attr[i], PTHREAD_EXPLICIT_SCHED);

        if(i == task_with_biggest_period)
        {
            struct job_timer_data *job_rel_tim = malloc(sizeof(struct job_timer_data));
            job_rel_tim->task_number = i;
            job_rel_tim->period_in_usec = tasks_data[i]->period;
            job_rel_tim->max_number_of_jobs = 100;
            result = pthread_create(&job_release_threads[i], &attr[i], &job_release_func_bounded, (void*) job_rel_tim);
        }
        else
        {
            struct job_timer_data *job_rel_tim = malloc(sizeof(struct job_timer_data));
            job_rel_tim->task_number = i;
            job_rel_tim->period_in_usec = tasks_data[i]->period;
            job_rel_tim->max_number_of_jobs = 100;
            result = pthread_create(&job_release_threads[i], &attr[i], &job_release_func, (void*) job_rel_tim);
        }
    }
    
    //Wait for the slowest task to finish
    pthread_join(job_release_threads[task_with_biggest_period], NULL);

    //cancel release threads of other tasks
    for(int i = 0; i < 4; i++)
    {
        result = pthread_cancel(job_release_threads[i]);
    }

    //wait for completition of already released jobs
    struct timespec wait_struct;
    clock_gettime(CLOCK_REALTIME, &wait_struct);

    wait_struct.tv_nsec += 200000000;
    if(wait_struct.tv_nsec >= 1000000000)
    {
        wait_struct.tv_nsec -=1000000000;
        wait_struct.tv_sec++;
    }
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wait_struct, NULL);
    

    //Generate JSON file
    print_log_data_json2(tasks_data[0], number_of_tasks);
    


    //CLose log
    closelog();

}


void *job_release_func_bounded(void *arguments)
{
    struct timespec next_activation;
    int result = 0;
    int number_of_releases = 0;
    struct job_timer_data *tim_data = (struct job_timer_data*) arguments;

    int nano_seconds = tim_data->period_in_usec * 1000;

    clock_gettime(CLOCK_REALTIME, &next_activation);

    log_first_activation(tim_data->task_number +1, next_activation);

    while(true)
    {
        
        next_activation.tv_nsec += nano_seconds;
        if(next_activation.tv_nsec >= 1000000000)
        {
            next_activation.tv_nsec -=1000000000;
            next_activation.tv_sec++;
        }

        result = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next_activation, NULL);
    
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


void *job_release_func(void *arguments)
{
    struct timespec next_activation;
    int result = 0;
    int number_of_releases = 0;
    struct job_timer_data *tim_data = (struct job_timer_data*) arguments;

    int nano_seconds = tim_data->period_in_usec * 1000;

    clock_gettime(CLOCK_REALTIME, &next_activation);


    log_first_activation(tim_data->task_number+1, next_activation);

    
    while(true)
    {

        next_activation.tv_nsec += nano_seconds;
        if(next_activation.tv_nsec >= 1000000000)
        {
            next_activation.tv_nsec -=1000000000;
            next_activation.tv_sec++;
        }

        result = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next_activation, NULL);
    
        ReleaseNewJob(tasks_data[tim_data->task_number]);

        number_of_releases ++;

        
    }

}

