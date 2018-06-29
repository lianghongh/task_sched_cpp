//
// Created by lianghong on 18-6-20.
//
#include <queue>
#include "cuckoo.h"
#include "../sched/pe_info.h"
#include "ga_tools.h"

#define PI 3.1415926

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
        if(isFeasible(g,individual))
        {
            power_cost(g, individual);
            nests.push_back(individual);
            n++;
        }
    }
}

int get_best_nest(TaskGraph &g,std::vector<Individual> &nest)
{
    power_cost(g, nest[0]);
    double min_cost=nest[0].fitness;
    int min_index=0;
    for(int i=1;i<nest.size();i++)
    {
        power_cost(g,nest[i]);
        if(nest[i].fitness<min_cost)
        {
            min_cost=nest[i].fitness;
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

void cuckoo(TaskGraph &g,std::vector<Individual> &nest,int best_index, double beta, double alpha)
{
    double sigma=pow(tgamma(1+beta)*sin(PI*beta/2)/(tgamma((1+beta)/2)*beta*pow(2,(beta-1)/2)),1/beta);
    std::normal_distribution<double> normal(0,1);
    for(int i=0;i<nest.size();i++)
    {
        do{
            double u = normal(e), v = normal(e);
            double step_size = u * sigma / pow(fabs(v), 1 / beta) * alpha;
            u = normal(e);
            for (int j = 0; j < nest[i].v.size(); j++) {
                nest[i].v[j].pe_index = check_pe(nest[i].v[j].pe_index + step_size * (nest[i].v[j].pe_index - nest[best_index].v[j].pe_index) * u);
                nest[i].v[j].voltage_level = check_voltage(nest[i].v[j].voltage_level + step_size * (nest[i].v[j].voltage_level - nest[best_index].v[j].voltage_level) * u);
            }
        } while (!isFeasible(g, nest[i]));
    }
}

void empty_nest(TaskGraph &g,std::vector<Individual> &nest,double pa)
{
    std::vector<Individual> nest1=nest;
    std::vector<Individual> nest2=nest;
    std::normal_distribution<double> normal_distribution(0,1);
    for(int i=0;i<nest.size();i++)
    {
        if(pa<real(e))
        {
            do {
                std::shuffle(nest1.begin(),nest1.end(),e);
                std::shuffle(nest2.begin(), nest2.end(),e);
                double k = normal_distribution(e);
                for (int j = 0; j < nest[i].v.size(); j++) {
                    nest[i].v[j].pe_index = check_pe(nest[i].v[j].pe_index + k * (nest1[i].v[j].pe_index - nest2[i].v[j].pe_index));
                    nest[i].v[j].voltage_level = check_voltage(nest[i].v[j].voltage_level + k * (nest1[i].v[j].voltage_level - nest2[i].v[j].voltage_level));
                }
            } while (!isFeasible(g, nest[i]));
        }
    }
}

void cuckoo_search(TaskGraph &g,int pop_size,int max_generation,double pa, double alpha,double beta)
{
    Individual best;
    std::vector<Individual> nest;
    init_random(g.task_num);
    cs_init_nest(g,nest, pop_size);
    int best_index = get_best_nest(g, nest);
    best = nest[best_index];

    std::cout<<"Gen 0\n";
    show_individual(best);
    std::cout<<"\n---------------------------------------------------------------------------------------------------------------------\n\n";

    for(int n=1;n<=max_generation;n++)
    {
        int index;
        cuckoo(g,nest,best_index,beta,alpha);
        index = get_best_nest(g, nest);
        empty_nest(g,nest,pa);
        index = get_best_nest(g, nest);

        if(nest[index].fitness<best.fitness)
        {
            best_index=index;
            best = nest[index];
        }

        std::cout<<"Gen "<<n<<"\n";
        show_individual(best);
        std::cout<<"\n---------------------------------------------------------------------------------------------------------------------\n\n";
    }
}