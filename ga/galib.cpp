//
// Created by lianghong on 2018/6/7.
//

#include "galib.h"

std::uniform_int_distribution<int> task_u,pe_u,voltage_u;
std::default_random_engine e;

void mutate_func1(Individual&);
void mutate_func2(Individual&);
void mutate_func3(Individual&);
void mutate_func4(Individual&);

std::vector<Mutate_func > func_list={mutate_func1,mutate_func2,mutate_func3,mutate_func4};

void init(Individual &v)
{
    task_u = std::uniform_int_distribution<int>(0, v.v.size() - 1);
    pe_u=std::uniform_int_distribution<int>(0,PE_COUNT-1);
    voltage_u = std::uniform_int_distribution<int>(0, VOLTAGE_LEVEL_COUNT - 1);
    e.seed((unsigned) time(0));
}


void cross_over(Individual&v1,Individual &v2)
{
    int cross_point1,cross_point2;
    do{
        cross_point1=task_u(e);
        cross_point2=task_u(e);
    }while (cross_point1==cross_point2);

    if(cross_point1>cross_point2)
    {
        int t=cross_point1;
        cross_point1=cross_point2;
        cross_point2=t;
    }
    for(int i=cross_point1;i<=cross_point2;i++)
    {
        Attribute b=v1.v[i];
        v1.v[i]=v2.v[i];
        v2.v[i]=b;
    }
}

void mutate_func1(Individual &v)
{
    int mutate=task_u(e);
    v.v[mutate].pe_index=pe_u(e);
    v.v[mutate].voltage_level=voltage_u(e);
}

void mutate_func2(Individual &v)
{
    int p1,p2;
    do{
        p1=task_u(e);
        p2=task_u(e);
    }while (p1==p2);
    if(p1>p2)
    {
        int t=p1;
        p1=p2;
        p2=t;
    }
    Attribute b=v.v[p1];
    v.v[p1]=v.v[p2];
    v.v[p2]=b;
}

void mutate_func3(Individual &v)
{
    int mutate=task_u(e);
    v.v[mutate].voltage_level=voltage_u(e);
}

void mutate_func4(Individual &v)
{
    int mutate=task_u(e);
    v.v[mutate].pe_index=pe_u(e);
}

void mutate(Individual &v)
{
    std::uniform_int_distribution<int> u(0, func_list.size() - 1);
    func_list[u(e)](v);
}
