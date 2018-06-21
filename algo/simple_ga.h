//
// Created by lianghong on 18-6-19.
//

#ifndef TASK_SCHED_SIMPLE_GA_H
#define TASK_SCHED_SIMPLE_GA_H


#include <vector>
#include "ga_tools.h"
#include "../sched/task_graph.h"

std::vector<Individual> simple_ga_select(std::vector<Individual> &population);

void simple_ga_crossover(Individual &parent1, Individual &parent2);

void simple_ga_mutate(Individual &in);

void simple_ga_init_population(TaskGraph &g,std::vector<Individual> &population, int npop);

void simple_ga(TaskGraph &g, int pop_size, int max_generation, double p_mute, double p_cross);

double start_time(TaskGraph &g,Individual &in,int task);
double finish_time(TaskGraph &g,Individual &in,int task);
double pe_ready(TaskGraph &g,Individual &in,int task);

bool isFeasible(TaskGraph &g,Individual &v);

double simple_ga_cost(TaskGraph &g,Individual &v);

#endif //TASK_SCHED_SIMPLE_GA_H
