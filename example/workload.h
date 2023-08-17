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

volatile u_int32_t dump11[10000];
volatile u_int32_t dump12[10000];
volatile u_int32_t dump13[10000];
volatile u_int32_t dump14[10000];
volatile u_int32_t dump15[10000];
volatile u_int32_t dump16[10000];
volatile u_int32_t dump17[10000];


volatile u_int32_t conv_array_t2_n1[10000];
volatile u_int32_t conv_array_t2_n2[10000];
volatile u_int32_t conv_array_t2_n3[10000];
volatile u_int32_t conv_array_t2_n4[10000];
volatile u_int32_t conv_array_t2_n5[10000];

volatile u_int32_t dump21[10000];
volatile u_int32_t dump22[10000];
volatile u_int32_t dump23[10000];
volatile u_int32_t dump24[10000];
volatile u_int32_t dump25[10000];




void create_workload();
void one_time_unit_workload(volatile u_int32_t *source_place, volatile u_int32_t *save_place);
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
