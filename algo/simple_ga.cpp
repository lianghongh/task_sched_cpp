//
// Created by lianghong on 18-6-19.
//
#include <queue>
#include "simple_ga.h"
#include "ga_tools.h"
#include "../sched/pe_info.h"
#include <algorithm>

bool compare(Individual &a1,Individual &a2)
{
    return a1.fitness<a2.fitness;
}

std::vector<Individual> simple_ga_select(std::vector<Individual> &population)
{
    std::vector<Individual> new_pop;
    std::sort(population.begin(), population.end(), compare);
    double total=0;
    for(int i=0;i<population.size();i++)
        total+=1000/population[i].fitness;
    for(int i=0;i<population.size();i++)
    {
        double select_p=real(e),p=0;
        for(int j=0;j<population.size();j++)
        {
            p+=1000/population[j].fitness/total;
            if(select_p<p)
            {
                new_pop.push_back(population[j]);
                break;
            }
        }
    }
    return new_pop;
}

void simple_ga_crossover(Individual &parent1, Individual &parent2)
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
        Attribute b=parent1.v[i];
        parent1.v[i]=parent2.v[i];
        parent2.v[i]=b;
    }
}

void simple_ga_mutate(Individual &in)
{
    int mutate=task_u(e);
    in.v[mutate].pe_index=pe_u(e);
    in.v[mutate].voltage_level = voltage_u(e);
}

double simple_ga_cost(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, Individual &v,
                int arc_index) {
    double cost = 0;
    for (int i = 0; i < v.v.size(); i++) {
        double new_voltage = voltage_level[v.v[i].pe_index][v.v[i].voltage_level];
        double factor = new_voltage / voltage_level[v.v[i].pe_index][VOLTAGE_LEVEL_COUNT - 1];
        cost += pe_dict[v.v[i].pe_index].pe_dict[g.nodes[v.v[i].task_index].type] * pe_dict[v.v[i].pe_index].power *
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
                cost += v.v[arr].pe_index != v.v[i].pe_index ? arc_dict[arc_index].power *
                                                               arc_dict[arc_index].arc_dict[p->type] : 0;
            } else {
                printf("can't find right node\n");
                exit(1);
            }
        }
    }
    return v.fitness = cost;
}

void simple_ga_init_population(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int arc_index,
                         std::vector<Individual> &population, int npop) {
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
        if (isFeasible(g,pe_dict,arc_dict,individual,arc_index)) {
            simple_ga_cost(g, pe_dict, arc_dict, individual, arc_index);
            population.push_back(individual);
            n++;
        }
    }
}

bool isFeasible(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &v,int arc_index=0)
{
    for(int i=0;i<v.v.size();i++)
    {
        run_queue[v.v[i].pe_index].push_back(v.v[i].task_index);
        v.v[i].start_time=v.v[i].finish_time=-1;
    }

    for(int i=0;i<v.v.size();i++)
    {
        if(v.v[i].finish_time==-1)
            v.v[i].finish_time=finish_time(g,pe_dict,arc_dict,v,v.v[i].task_index,arc_index);
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

double finish_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index)
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
    double t_min=pe_dict[pe_index].pe_dict[g.nodes[task].type];
    double w=t_min*pow(voltage_level[pe_index][VOLTAGE_LEVEL_COUNT-1]-threshold_voltage[pe_index],2)*voltage_level[pe_index][v_level]/(pow(voltage_level[pe_index][v_level]-threshold_voltage[pe_index],2)*voltage_level[pe_index][VOLTAGE_LEVEL_COUNT-1]);
    if(in.v[task].start_time==-1)
        in.v[task].start_time=start_time(g,pe_dict,arc_dict,in,task,arc_index);
    return w+in.v[task].start_time;
}

double pe_ready(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index)
{
    int pe_index=getPe(in,task);
    if(run_queue[pe_index][0]==task)
        return 0;
    int last_task;
    for(last_task=0;run_queue[pe_index][last_task]!=task;last_task++);
    double max_com=0;
    for(ArcNode *p=g.nodes[run_queue[pe_index][last_task-1]].next;p!= nullptr;p=p->next)
    {
        double arc_cost=pe_index!=getPe(in,p->task_index)?arc_dict[arc_index].arc_dict[p->type]:0;
        if(arc_cost>max_com)
            max_com=arc_cost;
    }
    return max_com+finish_time(g,pe_dict,arc_dict,in,run_queue[pe_index][last_task-1],arc_index);
}


double start_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task, int arc_index)
{
    double max_ft=0;
    for(ArcNode *p=g.nodes[task].pre;p!= nullptr;p=p->next)
    {
        if(in.v[p->task_index].finish_time==-1)
            in.v[p->task_index].finish_time=finish_time(g,pe_dict,arc_dict,in,p->task_index,arc_index);
        double arrive_time=in.v[p->task_index].finish_time;
        if(getPe(in,task)!=getPe(in,p->task_index))
            arrive_time+=arc_dict[arc_index].arc_dict[p->type];
        if(arrive_time>max_ft)
            max_ft=arrive_time;
    }
    double pe_time = pe_ready(g, pe_dict, arc_dict, in, task, arc_index);
    return pe_time>max_ft?pe_time:max_ft;
}


void simple_ga(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, int max_generation,
               double p_mute, double p_cross,int arc_index)
{
    double min_cost=INT32_MAX;
    Individual best;
    std::vector<Individual> population;
    init_random(g.task_num);
    simple_ga_init_population(g, pe_dict, arc_dict, arc_index, population, pop_size);
    for (int i = 0; i < population.size(); i++)
    {
        if(population[i].fitness<min_cost)
        {
            min_cost=population[i].fitness;
            best=population[i];
        }
    }
    std::cout<<"generation 0\n";
    show_individual(best);
    std::cout << "cost: " << best.fitness << "\n";
    std::cout<<"\n---------------------------------------------------------------------------------------------------------------------\n";

    for(int n=1;n<=max_generation;n++)
    {
        population = simple_ga_select(population);
        for(int i=0;i<population.size();i+=2)
        {
            if(real(e)<p_cross)
            {
                if(i+1<population.size())
                {
                    simple_ga_crossover(population[i],population[i+1]);
                    simple_ga_cost(g, pe_dict, arc_dict, population[i], arc_index);
                    simple_ga_cost(g, pe_dict, arc_dict, population[i + 1], arc_index);
                }
            }
        }
        for(int i=0;i<population.size();i++)
        {

            if(real(e)<p_mute)
            {
                simple_ga_mutate(population[i]);
                simple_ga_cost(g, pe_dict, arc_dict, population[i], arc_index);
            }
        }
        for (int i = 0; i < population.size(); i++) {
            if(population[i].fitness<min_cost&&isFeasible(g,pe_dict,arc_dict,population[i],arc_index))
            {
                min_cost=population[i].fitness;
                best=population[i];
            }
        }

        std::cout<<"generation "<<n<<"\n";
        show_individual(best);
        std::cout << "cost: " << best.fitness << "\n";
        std::cout<<"---------------------------------------------------------------------------------------------------------------------\n\n";
    }
}