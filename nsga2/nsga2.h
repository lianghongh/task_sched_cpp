//
// Created by lianghong on 18-6-29.
//

#ifndef TASK_SCHED_NSGA2_H
#define TASK_SCHED_NSGA2_H


#include <vector>
#include "../algo/ga_tools.h"
#define OBJECT_COUNT 2

typedef struct{
    std::vector<Individual> population;
    std::vector<std::vector<Individual> > fronts;
}Population;

extern double max_object[OBJECT_COUNT],min_object[OBJECT_COUNT];

void generateIndividual(TaskGraph &g,Individual &individual);

void cal_objective(TaskGraph &g,Individual &individual);

bool dominates(TaskGraph &g,Individual &in1,Individual &in2);

void fast_nondominate_sort(TaskGraph &g,Population &pop);

void nsga_crossover(Individual &parent1, Individual &parent2);

void nsga_mutate(Individual &in);

int tournament(Population &pop, int candidate);

void create_children(TaskGraph &g,Population &parent,Population &child, double cp, double mp,int candidate=3);

bool crowd_operator(Individual &in,Individual &other);

void init_population(TaskGraph &g,Population &pop,int pop_size);

void NSGA2(TaskGraph &g,int pop_size, int max_generation,double cp, double mp);

#endif //TASK_SCHED_NSGA2_H