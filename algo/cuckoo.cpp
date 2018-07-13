//
// Created by lianghong on 18-6-20.
//
#include <queue>
#include <algorithm>
#include "cuckoo.h"
#include "../sched/pe_info.h"
#include "ga_tools.h"
#include <fstream>

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
        power_cost(g, individual);
        time_cost(g, individual);
        constraint(g, individual);
        nests.push_back(individual);
        n++;
    }
}

bool is_dominate(TaskGraph &g,Individual &in1,Individual &in2)
{
    if(in1.constraint==0&&in2.constraint>0)
        return true;
    else if(in1.constraint>0&&in2.constraint>0&&in1.constraint<in2.constraint)
        return true;
    else if(in1.constraint==0&&in2.constraint==0)
    {
        return in1.power<in2.power;
    }

    return false;
}

int get_best_nest(TaskGraph &g,std::vector<Individual> &nest,std::vector<Individual> &new_nest)
{
    for(int i=0;i<nest.size();i++)
    {
        if(is_dominate(g,new_nest[i],nest[i]))
            nest[i] = new_nest[i];
    }
    int min=INT32_MAX,index=-1;
    for(int i=0;i<nest.size();i++)
    {
        nest[i].dominate_count=0;
        for(int j=0;j<nest.size();j++)
        {
            if(is_dominate(g,nest[j],nest[i]))
                nest[i].dominate_count++;
        }
        if(nest[i].dominate_count<min)
        {
            min=nest[i].dominate_count;
            index=i;
        }
    }
    return index;
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

void Levy(TaskGraph &g, std::vector<Individual> &nest, std::vector<Individual> &new_nest, Individual &best, double beta,
          double alpha)
{
    new_nest=nest;
    double sigma=pow(tgamma(1+beta)*sin(PI*beta/2)/(tgamma((1+beta)/2)*beta*pow(2,(beta-1)/2)),1/beta);
    std::normal_distribution<double> normal(0,1);
    for(int i=0;i<new_nest.size();i++)
    {
        double u = normal(e), v = normal(e);
        double step_size = u * sigma / pow(fabs(v), 1 / beta) * alpha;
        u = normal(e);
        for (int j = 0; j < new_nest[i].v.size(); j++)
        {
            new_nest[i].v[j].pe_index = check_pe(new_nest[i].v[j].pe_index + step_size * (new_nest[i].v[j].pe_index - best.v[j].pe_index) * u);
            new_nest[i].v[j].voltage_level = check_voltage(new_nest[i].v[j].voltage_level + step_size * (new_nest[i].v[j].voltage_level - best.v[j].voltage_level) * u);
        }
        power_cost(g,new_nest[i]);
        time_cost(g, new_nest[i]);
        constraint(g, new_nest[i]);
    }
}

void empty_nest(TaskGraph &g,std::vector<Individual> &nest,std::vector<Individual> &new_nest,double pa)
{
    new_nest=nest;
    std::vector<Individual> nest1=nest;
    std::vector<Individual> nest2=nest;
    std::normal_distribution<double> normal_distribution(0,1);
    std::shuffle(nest1.begin(),nest1.end(),e);
    std::shuffle(nest2.begin(), nest2.end(),e);
    for(int i=0;i<new_nest.size();i++)
    {
        if(pa<real(e))
        {
            double k = normal_distribution(e);
            for (int j = 0; j < new_nest[i].v.size(); j++)
            {
                new_nest[i].v[j].pe_index = check_pe(new_nest[i].v[j].pe_index + k * (nest1[i].v[j].pe_index - nest2[i].v[j].pe_index));
                new_nest[i].v[j].voltage_level = check_voltage(new_nest[i].v[j].voltage_level + k * (nest1[i].v[j].voltage_level - nest2[i].v[j].voltage_level));
            }
            power_cost(g,new_nest[i]);
            time_cost(g, new_nest[i]);
            constraint(g, new_nest[i]);
        }
    }
}

void cuckoo_search(TaskGraph &g,int pop_size,int max_generation,std::string path,double pa, double alpha,double beta)
{
    std::ofstream f(path,std::ios::out);
    if(!f.is_open())
    {
        std::cout<<"Can't open file-->"<<path<<"\n";
        return;
    }
    std::vector<Individual> nest;
    init_random(g.task_num);
    cs_init_nest(g,nest, pop_size);
    int best_index = get_best_nest(g, nest,nest);
    Individual best = nest[best_index];

    std::cout<<"*************************************\nStarting Cuckoo Search Algorithm...\n*************************************\n";
    std::cout<<"Gen 0\n";
    std::cout << best << "\n";
    f<<best<<"\n";

    std::vector<Individual> new_nest=nest;
    for(int n=1;n<=max_generation;n++)
    {
        Levy(g, nest, new_nest, best, beta, alpha);
        get_best_nest(g, nest,new_nest);
        empty_nest(g,nest,new_nest,pa);
        int index = get_best_nest(g, nest,new_nest);
        if(is_dominate(g,nest[index],best))
            best = nest[index];

        std::cout<<"Gen "<<n<<"\n";
        std::cout << best << "\n";
        f<<best<<"\n";
    }
    f.close();
}