#include "workload.h"

void create_workload()
{
	for(int p = 0; p <10000; p++)
    {
        conv_array[p] = p+18; 
    }
 }

void one_time_unit_workload(volatile u_int32_t *save_place)
{
    volatile u_int32_t *ptr = NULL;
    
    
    for(int p = 0; p < 10000; p++)
    {
        int temp_value = 0;
        
        
        for(int small_ind = 0; small_ind <10; small_ind++)
        {
            
            if(small_ind == 0)
            {
                temp_value = temp_value + conv_array[p];
            }
            else
            {
                int index = p - small_ind;
                if(index >= 0)
                {
                    temp_value = temp_value + conv_array[index];
                }
            
                index = p + small_ind;
            
                if(index <= 9999)
                {
                    temp_value = temp_value + conv_array[index];
                }
                
            } 
            
            
        }
        
        
        temp_value = temp_value/19;
        
        ptr = save_place + p;
        
        *ptr = temp_value;
        
    }
}

void node_1_1()
{
    volatile u_int32_t *ptr = &conv_array_t1_n1[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_2()
{
    volatile u_int32_t *ptr = &conv_array_t1_n2[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_3()
{
    volatile u_int32_t *ptr = &conv_array_t1_n3[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_4()
{
    volatile u_int32_t *ptr = &conv_array_t1_n4[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_5()
{
    volatile u_int32_t *ptr = &conv_array_t1_n5[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_6()
{
    volatile u_int32_t *ptr = &conv_array_t1_n6[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_1_7()
{
    volatile u_int32_t *ptr = &conv_array_t1_n7[0];
    
    one_time_unit_workload(ptr);
    one_time_unit_workload(ptr);
}

void node_2_1()
{

}

void node_2_2()
{

}

void node_2_3()
{

}

void node_2_4()
{

}

void node_2_5()
{

}
