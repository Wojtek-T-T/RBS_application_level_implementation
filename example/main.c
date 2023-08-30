#include "sequences.h"

#include <signal.h>
#include <sys/time.h>

#define task1_period 10
#define task2_period 20
#define task1_max_rel 1
#define task2_max_rel 0

void handler(int signo);
void handler2(int signo);

int task1_counter= 0;
int task2_counter= 0;

sem_t stop_sem;

int main(void) 
{    
    int result = 0;
    
    //Create an array for convolution
    create_workload();
    

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




    // SET TIMER FOR PERIODIC RELEASES OF JOBS
    signal(SIGALRM, handler);
    struct itimerval job_release_timer;
    job_release_timer.it_interval.tv_usec = 10000;
    job_release_timer.it_interval.tv_sec = 0;
    job_release_timer.it_value.tv_usec = 10000;
    job_release_timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &job_release_timer, NULL);



    sem_wait(&stop_sem);

    //CLose log
    closelog();

}

void handler(int signo)
{
    if(task1_counter == 0 && task1_data.job_counter != task1_max_rel)
    {
        ReleaseNewJob(&task1_data);
        task1_counter = task1_period;
    }
    else if(task1_data.job_counter == task1_max_rel)
    {
        if(task2_data.job_counter == task2_max_rel)
        {
            signal(SIGALRM, handler2);
            struct itimerval job_release_timer;
            job_release_timer.it_interval.tv_usec = 900000;
            job_release_timer.it_interval.tv_sec = 0;
            job_release_timer.it_value.tv_usec = 900000;
            job_release_timer.it_value.tv_sec = 0;
            setitimer(ITIMER_REAL, &job_release_timer, NULL);
            return;
        }
    }
    else
    {
        task1_counter--;
    }



    if(task2_counter == 0 && task2_data.job_counter != task2_max_rel)
    {
        //ReleaseNewJob(&task2_data);
        task2_counter = task2_period;
    }
    else if(task2_data.job_counter == task2_max_rel)
    {
        if(task1_data.job_counter == task1_max_rel)
        {
            signal(SIGALRM, handler2);
            struct itimerval job_release_timer;
            job_release_timer.it_interval.tv_usec = 900000;
            job_release_timer.it_interval.tv_sec = 0;
            job_release_timer.it_value.tv_usec = 900000;
            job_release_timer.it_value.tv_sec = 0;
            setitimer(ITIMER_REAL, &job_release_timer, NULL);
            return;
        }       
    }
    else
    {
        task2_counter--;
    }
}


void handler2(int signo)
{
    struct itimerval job_release_timer;
    job_release_timer.it_interval.tv_usec = 0;
    job_release_timer.it_interval.tv_sec = 0;
    job_release_timer.it_value.tv_usec = 0;
    job_release_timer.it_value.tv_sec = 0;
    setitimer(ITIMER_REAL, &job_release_timer, NULL);

    display_log_data();
    sem_post(&stop_sem);
}


