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

void handler(int sig, siginfo_t *si, void *uc);
void set_release_timer(int time_usec, int time_sec, int timer_id);
void *timer_thread_func(void *arguments);



timer_t timerIds[20];


sem_t *semaphore_stop;
pthread_t timer_thread;


int main(void) 
{    

    int result = 0;
    semaphore_stop = malloc(sizeof(sem_t));
    result = sem_init(semaphore_stop, 0, 0);

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
    
    result = pthread_create(&timer_thread, NULL, &timer_thread_func, (void*) "0");


    pthread_join(timer_thread, NULL);


    print_log_data_txt();

    //CLose log
    closelog();

}

void handler(int sig, siginfo_t *si, void *uc)
{
    
    if(si->si_value.sival_ptr == &timerIds[0])
    {
        ReleaseNewJob(tasks_data[0]);
    }
    else if(si->si_value.sival_ptr == &timerIds[1])
    {
        ReleaseNewJob(tasks_data[1]);
    }
    else if(si->si_value.sival_ptr == &timerIds[2])
    {
        ReleaseNewJob(tasks_data[2]);
    }
    else if(si->si_value.sival_ptr == &timerIds[3])
    {
        ReleaseNewJob(tasks_data[3]);
    }
    else if(si->si_value.sival_ptr == &timerIds[4])
    {
        ReleaseNewJob(tasks_data[4]);
    }
    else if(si->si_value.sival_ptr == &timerIds[5])
    {
        ReleaseNewJob(tasks_data[5]);
    }
    else if(si->si_value.sival_ptr == &timerIds[6])
    {
        ReleaseNewJob(tasks_data[6]);
    }
    else if(si->si_value.sival_ptr == &timerIds[7])
    {
        ReleaseNewJob(tasks_data[7]);
    }
    else if(si->si_value.sival_ptr == &timerIds[19])
    {
        sem_post(semaphore_stop);
    }
    else{
        printf("unknown interrupr\n");
    }

    
}

void set_release_timer(int time_usec, int time_sec, int timer_id)
{
    int res = 0;
    int period_nsec = 0;
    int period_sec = 0;

    if(time_sec != 0)
    {
        period_sec = time_sec;
        period_nsec = time_usec * 1000;
    }
    else
    {
        period_nsec = time_usec * 1000;
    }
    

    struct sigevent sev;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerIds[timer_id];
    res = timer_create(CLOCK_REALTIME, &sev, &timerIds[timer_id]);


    struct itimerspec job_release_timer;
    job_release_timer.it_interval.tv_nsec = period_nsec;
    job_release_timer.it_interval.tv_sec = period_sec;
    job_release_timer.it_value.tv_nsec = period_nsec;
    job_release_timer.it_value.tv_sec = period_sec;

    timer_settime(timerIds[timer_id], 0 ,&job_release_timer, NULL);

}

void *timer_thread_func(void *arguments)
{
    for(int i = 0; i < number_of_tasks; i++)
    {
        set_release_timer((tasks_data[i]->period), 0, i);
    }

    set_release_timer(0, 3, 19);

    sem_wait(semaphore_stop);
}
