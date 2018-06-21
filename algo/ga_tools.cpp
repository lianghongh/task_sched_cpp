//
// Created by lianghong on 18-6-19.
//
#include "ga_tools.h"
#include "../sched/pe_info.h"

std::uniform_int_distribution<int> task_u, pe_u, voltage_u;
std::uniform_real_distribution<double> real;

std::default_random_engine e;

void init_random(int task_size) {
    task_u = std::uniform_int_distribution<int>(0, task_size - 1);
    pe_u = std::uniform_int_distribution<int>(0, PE_COUNT - 1);
    voltage_u = std::uniform_int_distribution<int>(0, VOLTAGE_LEVEL_COUNT - 1);
    real = std::uniform_real_distribution<double>(0, 1);
    e.seed((unsigned) time(0));
}

void show_individual(Individual &in)
{
    for(int i=0;i<in.v.size();i++)
    {
        printf("%6d ",in.v[i].task_index);
    }
    printf("\n");
    for(int i=0;i<in.v.size();i++)
    {
        printf("%6d ", in.v[i].pe_index);
    }
    printf("\n");
    for(int i=0;i<in.v.size();i++)
    {
        printf("%6.4f ",voltage_level[in.v[i].pe_index][in.v[i].voltage_level]);
    }
    printf("\ncost: %f\n",in.fitness);
}

int getPe(Individual& in,int task)
{
    for(int i=0;i<in.v.size();i++)
    {
        if(task==in.v[i].task_index)
            return in.v[i].pe_index;
    }
    return -1;
}