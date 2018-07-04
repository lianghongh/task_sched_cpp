//
// Created by lianghong on 18-6-19.
//
#include <queue>
#include <algorithm>
#include "simple_ga.h"

bool better(TaskGraph &g,Individual &in1,Individual &in2)
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

int choice(TaskGraph &g,std::vector<Individual> &pop,int candidate=3)
{
    std::vector<int> cac;
    std::uniform_int_distribution<int> u(0,pop.size()-1);
    for(int i=0;i<candidate;i++)
        cac.push_back(u(e));
    for(int i=0;i<cac.size();i++)
    {
        pop[cac[i]].dominate_count=0;
        for(int j=0;j<cac.size();j++)
        {
            if(better(g,pop[cac[j]],pop[cac[i]]))
                pop[cac[i]].dominate_count++;
        }
    }
    int index=-1,min=INT32_MAX;
    for(int i=0;i<cac.size();i++)
    {
        if(pop[cac[i]].dominate_count<min)
        {
            min=pop[cac[i]].dominate_count;
            index=i;
        }
    }
    return index;
}

void simple_ga_select(TaskGraph &g,std::vector<Individual> &population,std::vector<Individual> &new_pop, double cp,
                      double mp)
{
    while (new_pop.size()<population.size())
    {
        int parent1,parent2;
        do {
            parent1 = choice(g,population);
            parent2 = choice(g, population);
        } while (parent1 == parent2);

        Individual pp1 = population[parent1], pp2 = population[parent2];
        if(cp<real(e))
            simple_ga_crossover(pp1, pp2);
        if(mp<real(e))
        {
            simple_ga_mutate(pp1);
            simple_ga_mutate(pp2);
        }
        power_cost(g, pp1);
        time_cost(g, pp1);
        constraint(g, pp1);
        power_cost(g, pp2);
        time_cost(g, pp2);
        constraint(g, pp2);
        new_pop.push_back(pp1);
        new_pop.push_back(pp2);
    }
}

int get_best(TaskGraph &g,std::vector<Individual> &pop,std::vector<Individual> &new_pop)
{
    for(int i=0;i<pop.size();i++)
    {
        if(better(g,new_pop[i],pop[i]))
            pop[i] = new_pop[i];
        pop[i].dominate_count=0;
        for(int j=0;j<pop.size();j++)
        {
            if(better(g,pop[j],pop[i]))
                pop[i].dominate_count++;
        }
    }
    int min=INT32_MAX,index=-1;
    for(int i=0;i<pop.size();i++)
    {
        if(pop[i].dominate_count<min)
        {
            min=pop[i].dominate_count;
            index=i;
        }
    }
    return index;
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

void simple_ga_init_population(TaskGraph &g, std::vector<Individual> &population, int npop) {
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
        population.push_back(individual);
        n++;
    }
}

void simple_ga(TaskGraph &g,int pop_size, int max_generation,double p_mute, double p_cross)
{
    init_random(g.task_num);
    std::vector<Individual> population;
    simple_ga_init_population(g,population, pop_size);
    int best_index = get_best(g, population,population);
    Individual best = population[best_index];
    std::cout<<"Gen 0\n";
    std::cout << best << "\n";
    std::vector<Individual> new_pop;

    for(int n=1;n<=max_generation;n++)
    {
        simple_ga_select(g, population, new_pop, p_cross, p_mute);
        int index = get_best(g,population,new_pop);
        if(better(g,population[index],best))
            best=population[index];

        std::cout<<"Gen "<<n<<"\n";
        std::cout << best<< "\n";
        new_pop.clear();
    }
}