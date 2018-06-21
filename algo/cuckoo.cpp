//
// Created by lianghong on 18-6-20.
//
#include <queue>
#include "cuckoo.h"
#include "ga_tools.h"
#include "../sched/pe_info.h"
#include <cmath>
#define PI 3.1415926

double cs_cost(TaskGraph &g,Individual &v)
{
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
    return v.fitness=cost;
}


double cs_finish(TaskGraph &g,Individual &in,int task)
{
    int pe_index=0,v_level=0;
    for(int i=0;i<in.v.size();i++)
    {
        if(in.v[i].task_index==task)
        {
            pe_index=in.v[i].pe_index;
            v_level=in.v[i].voltage_level;
            break;
        }
    }
    double t_min=g.pe_dict[pe_index].pe_dict[g.nodes[task].type];
    double w=t_min*pow(voltage_level[pe_index][VOLTAGE_LEVEL_COUNT-1]-threshold_voltage[pe_index],2)*voltage_level[pe_index][v_level]/(pow(voltage_level[pe_index][v_level]-threshold_voltage[pe_index],2)*voltage_level[pe_index][VOLTAGE_LEVEL_COUNT-1]);
    if(in.v[task].start_time==-1)
        in.v[task].start_time=cs_start(g,in,task);
    return w+in.v[task].start_time;
}

double cs_pe(TaskGraph &g,Individual &in,int task)
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
    return max_com+cs_finish(g,in,run_queue[pe_index][last_task-1]);
}


double cs_start(TaskGraph &g,Individual &in,int task)
{
    double max_ft=0;
    for(ArcNode *p=g.nodes[task].pre;p!= nullptr;p=p->next)
    {
        if(in.v[p->task_index].finish_time==-1)
            in.v[p->task_index].finish_time=cs_finish(g,in,p->task_index);
        double arrive_time=in.v[p->task_index].finish_time;
        if(getPe(in,task)!=getPe(in,p->task_index))
            arrive_time+=g.arc_dict[g.arc_index].arc_dict[p->type];
        if(arrive_time>max_ft)
            max_ft=arrive_time;
    }
    double pe_time = cs_pe(g,in, task);
    return pe_time>max_ft?pe_time:max_ft;
}

bool cs_sched(TaskGraph &g,Individual &v)
{
    for(int i=0;i<v.v.size();i++)
    {
        run_queue[v.v[i].pe_index].push_back(v.v[i].task_index);
        v.v[i].start_time=v.v[i].finish_time=-1;
    }

    for(int i=0;i<v.v.size();i++)
    {
        if(v.v[i].finish_time==-1)
            v.v[i].finish_time=cs_finish(g,v,v.v[i].task_index);
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

void cs_init_nest(TaskGraph &g,std::vector<Individual> &nests, int npop)
{
    int n = 0;
    while (n < npop) {
        Individual individual;
        std::vector<TaskNode> task_set;
        std::queue<TaskNode> q;
        for (int i = 0; i < g.task_num; i++)
            task_set.push_back(g.nodes[i]);

        for (int i = 0; i<task_set.size(); i++) {
            if (task_set[i].in_degree == 0)
                q.push(task_set[i]);
        }

        while (!q.empty()) {
            TaskNode t = q.front();
            q.pop();
            Attribute aa;
            aa.task_index=t.task_index;
            aa.pe_index=pe_u(e);
            aa.voltage_level=voltage_u(e);
            aa.start_time=aa.finish_time=-1;
            individual.v.push_back(aa);
            for (ArcNode *p = t.next; p != nullptr; p = p->next)
            {
                if(--task_set[p->task_index].in_degree==0)
                    q.push(task_set[p->task_index]);
            }
        }
        if (cs_sched(g,individual)) {
            cs_cost(g,individual);
            nests.push_back(individual);
            n++;
        }
    }
}

int get_best_nest(TaskGraph &g,std::vector<Individual> &nest,std::vector<Individual> &new_nest,std::vector<double> &fitness)
{
    for(int i=0;i<new_nest.size();i++)
    {
        double f=cs_cost(g,new_nest[i]);
        if(f<fitness[i])
        {
            fitness[i]=f;
            nest[i]=new_nest[i];
        }
    }
    int min_index=0;
    double min_fitness=fitness[0];
    for(int i=1;i<fitness.size();i++)
    {
        if(fitness[i]<min_fitness)
        {
            min_fitness=fitness[i];
            min_index=i;
        }
    }
    return min_index;
}

int check_pe(double pe)
{
    if(pe<0)
        return 0;
    else if(pe>PE_COUNT-1)
        return PE_COUNT-1;

    return (int)round(pe);
}

int check_voltage(double voltage)
{
    if(voltage<0)
        return 0;
    else if(voltage>VOLTAGE_LEVEL_COUNT-1)
        return  VOLTAGE_LEVEL_COUNT-1;

    return (int) round(voltage);
}

void cuckoo(TaskGraph &g,std::vector<Individual> &nest,std::vector<Individual> &new_nest,int best_index, double beta=1.5, double alpha=0.01)
{
    new_nest=nest;
    double sigma=pow(tgamma(1+beta)*sin(PI*beta/2)/(tgamma((1+beta)/2)*beta*pow(2,(beta-1)/2)),1/beta);
    std::normal_distribution<double> normal(0,1);
    for(int i=0;i<nest.size();i++)
    {
        do {
            double u = normal(e), v = normal(e);
            double step_size = u * sigma / pow(fabs(v), 1 / beta) * alpha;
            u = normal(e);
            for (int j = 0; j < new_nest[i].v.size(); j++) {
                new_nest[i].v[j].pe_index = check_pe(new_nest[i].v[j].pe_index + step_size *
                                                                                 (new_nest[i].v[j].pe_index -
                                                                                  nest[best_index].v[j].pe_index) * u);
                new_nest[i].v[j].voltage_level = check_voltage(new_nest[i].v[j].voltage_level + step_size *
                                                                                                (new_nest[i].v[j].voltage_level -
                                                                                                 nest[best_index].v[j].voltage_level) *
                                                                                                u);
            }
        }while (!cs_sched(g,new_nest[i]));
    }
}

void empty_nest(TaskGraph &g,std::vector<Individual> &nests, std::vector<Individual> &new_nest,double pa)
{
    new_nest=nests;
    std::vector<Individual> nest1=nests;
    std::vector<Individual> nest2=nests;
    std::normal_distribution<double> normal_distribution(0,1);
    std::shuffle(nest1.begin(),nest1.end(),e);
    std::shuffle(nest2.begin(), nest2.end(),e);
    for(int i=0;i<nests.size();i++)
    {
        if(pa<normal_distribution(e))
        {
            double k=normal_distribution(e);
            do {
                for (int j = 0; j < new_nest[i].v.size(); j++) {
                    new_nest[i].v[j].pe_index = check_pe(
                            new_nest[i].v[j].pe_index + k * (nest1[i].v[j].pe_index - nest2[i].v[j].pe_index));
                    new_nest[i].v[j].voltage_level = check_voltage(new_nest[i].v[j].voltage_level + k *
                                                                                                    (nest1[i].v[j].voltage_level -
                                                                                                     nest2[i].v[j].voltage_level));
                }
            }while (!cs_sched(g,new_nest[i]));
        }
    }
}

void cuckoo_search(TaskGraph &g,int pop_size,int max_generation,double pa)
{
    double min_cost=INT32_MAX;
    Individual best;
    std::vector<Individual> nests;
    init_random(g.task_num);
    cs_init_nest(g,nests, pop_size);
    for(int i=0;i<nests.size();i++)
    {
        if(nests[i].fitness<min_cost)
        {
            min_cost=nests[i].fitness;
            best = nests[i];
        }
    }
    std::cout<<"Gen 0\n";
    show_individual(best);
    std::cout<<"\n---------------------------------------------------------------------------------------------------------------------\n\n";

    std::vector<double > fitness(nests.size(),INT32_MIN);
    int best_index = get_best_nest(g, nests, nests, fitness);
    std::vector<Individual> new_nest;
    for(int n=1;n<=max_generation;n++)
    {
        int index;
        cuckoo(g,nests,new_nest,best_index);
        index = get_best_nest(g, nests, new_nest, fitness);
        empty_nest(g,nests,new_nest,pa);
        index = get_best_nest(g, nests, new_nest, fitness);

        if(fitness[index]<min_cost)
        {
            best_index=index;
            best = nests[best_index];
        }
    }
}