#include "workload.h"

void create_workload()
{
	for(int p = 0; p <10000; p++)
    {
        conv_array_t1_n1[p] = p+18; 
        conv_array_t1_n2[p] = p+19; 
        conv_array_t1_n3[p] = p+20; 
        conv_array_t1_n4[p] = p+21; 
        conv_array_t1_n5[p] = p+22; 
        conv_array_t1_n6[p] = p+23; 
        conv_array_t1_n7[p] = p+24; 
    }
 }

void one_time_unit_workload(volatile u_int32_t *save_place)
{
    volatile u_int32_t *ptr = NULL;
    
    u_int32_t temp[10000];
    
    for(int p = 0; p < 10000; p++)
    {
        int temp_value = 0;
        ptr = save_place + p;
        
        
        for(int small_ind = 0; small_ind <10; small_ind++)
        {
            
            if(small_ind == 0)
            {
                temp_value = temp_value + *ptr;
            }
            else
            {
                int index = p - small_ind;
                
                ptr = ptr - small_ind;
                
                
                if(index >= 0)
                {
                    temp_value = temp_value + *ptr;
                }
            
                index = p + small_ind;
            
                if(index <= 9999)
                {
                    temp_value = temp_value + *ptr;
                }
                
            } 
            
            
        }
        
        
        temp_value = temp_value/19;
        
        temp[p] = temp_value;
        

        
    }
    
    for(int p = 0; p < 10000; p++)
    {
		ptr = save_place + p;
        
        *ptr = temp[p];
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
    
    printf("hallo\n");
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
