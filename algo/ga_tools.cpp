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
    printf("\ncost: %f\n",in.power);
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

int getIndex(Individual &in,int task)
{
    for(int i=0;i<in.v.size();i++)
    {
        if(task==in.v[i].task_index)
            return i;
    }
    return -1;
}

bool isFeasible(TaskGraph &g,Individual &v)
{
    for(int i=0;i<v.v.size();i++)
    {
        run_queue[v.v[i].pe_index].push_back(v.v[i].task_index);
        v.v[i].start_time=v.v[i].finish_time=-1;
    }

    for(int i=0;i<v.v.size();i++)
    {
        if(v.v[i].finish_time==-1)
            v.v[i].finish_time= finish(g, v, v.v[i].task_index);
        if(v.v[i].finish_time>g.nodes[v.v[i].task_index].deadline)
        {
            for(int j=0;j<PE_COUNT;j++)
                run_queue[j].clear();

            for(int j=0;j<v.v.size();j++)
                v.v[j].start_time=v.v[j].finish_time=-1;
            return false;
        }
    }
    for(int i=0;i<PE_COUNT;i++)
        run_queue[i].clear();
    for(int i=0;i<v.v.size();i++)
        v.v[i].start_time=v.v[i].finish_time=-1;
    return true;
}


double finish(TaskGraph &g, Individual &in, int task)
{
    int pe_index=-1,v_level=-1,index=-1;
    for(int i=0;i<in.v.size();i++)
    {
        if(in.v[i].task_index==task)
        {
            pe_index=in.v[i].pe_index;
            v_level=in.v[i].voltage_level;
            index=i;
            break;
        }
    }
    double t_min=g.pe_dict[pe_index].pe_dict[g.nodes[task].type];
    double w=t_min*pow(voltage_level[pe_index][VOLTAGE_LEVEL_COUNT-1]-threshold_voltage[pe_index],2)*voltage_level[pe_index][v_level]/(pow(voltage_level[pe_index][v_level]-threshold_voltage[pe_index],2)*voltage_level[pe_index][VOLTAGE_LEVEL_COUNT-1]);
    if(in.v[index].start_time==-1)
        in.v[index].start_time= start(g, in, task);
    return w+in.v[index].start_time;
}

double pe(TaskGraph &g, Individual &in, int task)
{
    int pe_index=getPe(in,task);
    if(run_queue[pe_index][0]==task)
        return 0;
    int last_task;
    for(last_task=0;run_queue[pe_index][last_task]!=task;last_task++);
    double max_com=0;
    for(ArcNode *p=g.nodes[run_queue[pe_index][last_task-1]].next;p!= nullptr;p=p->next)
    {
        double arc_cost=pe_index!=getPe(in,p->task_index)?g.arc_dict[g.arc_index].arc_dict[p->type]:0;
        if(arc_cost>max_com)
            max_com=arc_cost;
    }
    return max_com+ finish(g, in, run_queue[pe_index][last_task - 1]);
}


double start(TaskGraph &g, Individual &in, int task)
{
    double max_ft=0;
    for(ArcNode *p=g.nodes[task].pre;p!= nullptr;p=p->next)
    {
        int index=getIndex(in,p->task_index);
        if(in.v[index].finish_time==-1)
            in.v[index].finish_time= finish(g, in, p->task_index);
        double arrive_time=in.v[index].finish_time;
        if(getPe(in,task)!=getPe(in,p->task_index))
            arrive_time+=g.arc_dict[g.arc_index].arc_dict[p->type];
        if(arrive_time>max_ft)
            max_ft=arrive_time;
    }
    double pe_time = pe(g, in, task);
    return pe_time>max_ft?pe_time:max_ft;
}

double power_cost(TaskGraph &g,Individual &v) {
    double cost = 0;
    for (int i = 0; i < v.v.size(); i++) {
        double new_voltage = voltage_level[v.v[i].pe_index][v.v[i].voltage_level];
        double factor = new_voltage / voltage_level[v.v[i].pe_index][VOLTAGE_LEVEL_COUNT - 1];
        cost += g.pe_dict[v.v[i].pe_index].pe_dict[g.nodes[v.v[i].task_index].type] * g.pe_dict[v.v[i].pe_index].power *
                factor * factor;
        for (ArcNode *p = g.nodes[v.v[i].task_index].next; p != nullptr; p = p->next) {
            int arr = -1;
            for (int j = 0; j < v.v.size(); j++) {
                if (v.v[j].task_index == p->task_index) {
                    arr = j;
                    break;
                }
            }
            if (arr != -1) {
                cost += v.v[arr].pe_index != v.v[i].pe_index ? g.arc_dict[g.arc_index].power *
                                                               g.arc_dict[g.arc_index].arc_dict[p->type] : 0;
            } else {
                printf("can't find right node\n");
                exit(1);
            }
        }
    }
    return v.power = cost;
}

double time_cost(TaskGraph &g, Individual &v)
{
    for(int i=0;i<v.v.size();i++)
    {
        run_queue[v.v[i].pe_index].push_back(v.v[i].task_index);
        v.v[i].start_time=v.v[i].finish_time=-1;
    }
    double max=0;
    for(int i=0;i<v.v.size();i++)
    {
        if(v.v[i].finish_time==-1)
            v.v[i].finish_time= finish(g, v, v.v[i].task_index);
        if(v.v[i].finish_time>max)
            max=v.v[i].finish_time;
    }
    for(int i=0;i<PE_COUNT;i++)
        run_queue[i].clear();
    for(int i=0;i<v.v.size();i++)
        v.v[i].start_time=v.v[i].finish_time=-1;
    return v.time=max;
}
