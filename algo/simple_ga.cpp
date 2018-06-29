//
// Created by lianghong on 18-6-19.
//
#include <queue>
#include "simple_ga.h"

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
        if (isFeasible(g,individual)) {
            power_cost(g,individual);
            population.push_back(individual);
            n++;
        }
    }
}

void simple_ga(TaskGraph &g,int pop_size, int max_generation,double p_mute, double p_cross)
{
    double min_cost=INT32_MAX;
    Individual best;
    std::vector<Individual> population;
    init_random(g.task_num);
    simple_ga_init_population(g,population, pop_size);
    for (int i = 0; i < population.size(); i++)
    {
        if(population[i].fitness<min_cost)
        {
            min_cost=population[i].fitness;
            best=population[i];
        }
    }
    std::cout<<"Gen 0\n";
    show_individual(best);
    std::cout<<"\n---------------------------------------------------------------------------------------------------------------------\n\n";

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
                    power_cost(g, population[i]);
                    power_cost(g, population[i + 1]);
                }
            }
        }
        for(int i=0;i<population.size();i++)
        {

            if(real(e)<p_mute)
            {
                simple_ga_mutate(population[i]);
                power_cost(g,population[i]);
            }
        }
        for (int i = 0; i < population.size(); i++) {
            if(population[i].fitness<min_cost&&isFeasible(g,population[i]))
            {
                min_cost=population[i].fitness;
                best=population[i];
            }
        }

        std::cout<<"Gen "<<n<<"\n";
        show_individual(best);
        std::cout<<"\n---------------------------------------------------------------------------------------------------------------------\n\n";
    }
}