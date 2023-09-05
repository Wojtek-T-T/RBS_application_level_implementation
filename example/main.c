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

#define task1_period 10
#define task2_period 20
#define task1_max_rel 3
#define task2_max_rel 3

void handler(int sig, siginfo_t *si, void *uc);
void set_release_timer(int time_usec, int time_sec);
void *timer_thread_func(void *arguments);

int task1_counter= 0;
int task2_counter= 0;

timer_t timerIds[20];
int timer_id_counter = 0;

sem_t *semaphore_stop;
pthread_t timer_thread;

int main(void) 
{    

    int result = 0;
    semaphore_stop = malloc(sizeof(sem_t));
    result = sem_init(semaphore_stop, 0, 0);

    
    //Initialize RBS
    initialize_rbs();
    

    //Initialize tasks
    InitializeTask(&task1_data);
    InitializeTask(&task2_data);
    

    //initialize sequences
    InitializeSequence(&task1_data, 1, &task1_threads[0], task1_data.attr, seq_func_ptr_t1[0]);
    InitializeSequence(&task1_data, 2, &task1_threads[1], task1_data.attr, seq_func_ptr_t1[1]);
    InitializeSequence(&task1_data, 3, &task1_threads[2], task1_data.attr, seq_func_ptr_t1[2]);
    InitializeSequence(&task1_data, 4, &task1_threads[3], task1_data.attr, seq_func_ptr_t1[3]);
    
    InitializeSequence(&task2_data, 1, &task2_threads[0], task2_data.attr, seq_func_ptr_t2[0]);
    InitializeSequence(&task2_data, 2, &task2_threads[1], task2_data.attr, seq_func_ptr_t2[1]);
    InitializeSequence(&task2_data, 3, &task2_threads[2], task2_data.attr, seq_func_ptr_t2[2]);   

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
        ReleaseNewJob(&task1_data);
    }
    else if(si->si_value.sival_ptr == &timerIds[1])
    {
        ReleaseNewJob(&task2_data);
    }
    else if(si->si_value.sival_ptr == &timerIds[2])
    {
        sem_post(semaphore_stop);
    }
    else{
        printf("unknown interrupr\n");
    }

    
}

void set_release_timer(int time_usec, int time_sec)
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
    sev.sigev_value.sival_ptr = &timerIds[timer_id_counter];
    res = timer_create(CLOCK_REALTIME, &sev, &timerIds[timer_id_counter]);


    struct itimerspec job_release_timer;
    job_release_timer.it_interval.tv_nsec = period_nsec;
    job_release_timer.it_interval.tv_sec = period_sec;
    job_release_timer.it_value.tv_nsec = period_nsec;
    job_release_timer.it_value.tv_sec = period_sec;

    timer_settime(timerIds[timer_id_counter], 0 ,&job_release_timer, NULL);

    timer_id_counter++;
}

void *timer_thread_func(void *arguments)
{
    set_release_timer(117000, 0);
    set_release_timer(117063, 0);
    set_release_timer(0, 10);

    sem_wait(semaphore_stop);

}