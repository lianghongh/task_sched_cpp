//
// Created by lianghong on 18-6-19.
//
#include <queue>
#include "simple_ga.h"
#include "ga_tools.h"
#include "hga.h"
#include <time.h>

int simple_ga_select(std::vector<Individual> &population)
{
    double total=0;
    for(int i=0;i<population.size();i++)
        total+=population[i].fitness;
    int parent;
    double select_p=real(e),p=0;
    for(int i=0;i<population.size();i++)
    {
        p+=population[i].fitness/total;
        if(select_p<p)
        {
            parent=i;
            break;
        }
    }
    return parent;
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

Individual simple_ga_mutate(Individual &in)
{
    Individual v=in;
    int mutate=task_u(e);
    v.v[mutate].pe_index=pe_u(e);
    v.v[mutate].voltage_level = voltage_u(e);
    return v;
}


void simple_ga(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, int max_generation,
               double p_mute, double p_cross,int arc_index)
{
    double min_cost=INT32_MAX;
    Individual best;
    std::vector<Individual> population;
    init_random(g.task_num);
    hga_init_population(g, pe_dict, arc_dict, arc_index, population, pop_size);
    for (int i = 0; i < pop_size; i++)
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
        int parent1,parent2;
        do{
            parent1 = hga_select(population);
            parent2 = hga_select(population);
        }while (parent1==parent2);
        if(real(e)<p_cross)
            simple_ga_crossover(population[parent1], population[parent2]);
        if(real(e)<p_mute)
        {
            simple_ga_mutate(population[parent1]);
            simple_ga_mutate(population[parent2]);
        }
        hga_cost(g, pe_dict, arc_dict, population[parent1], arc_index);
        hga_cost(g, pe_dict, arc_dict, population[parent2], arc_index);
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

void simple_ga(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, unsigned exec_time,
               double p_mute, double p_cross,int arc_index)
{
    double min_cost=INT32_MAX;
    Individual best;
    std::vector<Individual> population;
    init_random(g.task_num);
    hga_init_population(g, pe_dict, arc_dict, arc_index, population, pop_size);
    for (int i = 0; i < pop_size; i++)
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

    unsigned end_time=(unsigned)time(0)+exec_time;
    while((unsigned)time(0)<end_time)
    {
        int parent1,parent2;
        do{
            parent1 = hga_select(population);
            parent2 = hga_select(population);
        }while (parent1==parent2);
        if(real(e)<p_cross)
            simple_ga_crossover(population[parent1], population[parent2]);
        if(real(e)<p_mute)
        {
            simple_ga_mutate(population[parent1]);
            simple_ga_mutate(population[parent2]);
        }
        hga_cost(g, pe_dict, arc_dict, population[parent1], arc_index);
        hga_cost(g, pe_dict, arc_dict, population[parent2], arc_index);

        for (int i = 0; i < population.size(); i++) {
            if(population[i].fitness<min_cost&&isFeasible(g,pe_dict,arc_dict,population[i],arc_index))
            {
                min_cost=population[i].fitness;
                best=population[i];
            }
        }
    }

    show_individual(best);
    std::cout << "cost: " << best.fitness << "\n";
    std::cout<<"---------------------------------------------------------------------------------------------------------------------\n\n";
}