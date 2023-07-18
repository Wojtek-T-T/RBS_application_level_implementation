#ifndef WORKLOAD_H
#define WORKLOAD_H


#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>

volatile u_int32_t conv_array[10000];

volatile u_int32_t conv_array_t1_n1[10000];
volatile u_int32_t conv_array_t1_n2[10000];
volatile u_int32_t conv_array_t1_n3[10000];
volatile u_int32_t conv_array_t1_n4[10000];
volatile u_int32_t conv_array_t1_n5[10000];
volatile u_int32_t conv_array_t1_n6[10000];
volatile u_int32_t conv_array_t1_n7[10000];


void create_workload();
void one_time_unit_workload(volatile u_int32_t *save_place);
void node_1_1();
void node_1_2();
void node_1_3();
void node_1_4();
void node_1_5();
void node_1_6();
void node_1_7();
void node_2_1();
void node_2_2();
void node_2_3();
void node_2_4();
void node_2_5();

#endif
