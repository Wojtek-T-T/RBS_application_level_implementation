#ifndef OTW_H
#define OTW_H

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

extern volatile int conv_array[10100];

extern int mask[11];

void one_time_unit_workload();

#endif