//
// Created by lianghong on 18-6-19.
//

#ifndef TASK_SCHED_SIMPLE_GA_H
#define TASK_SCHED_SIMPLE_GA_H


#include <vector>
#include "ga_tools.h"
#include "../sched/task_graph.h"
#include "../tgff/tgff_tools.h"

std::vector<Individual> simple_ga_select(std::vector<Individual> &population);

void simple_ga_crossover(Individual &parent1, Individual &parent2);

void simple_ga_mutate(Individual &in);

void simple_ga_init_population(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int arc_index,
                               std::vector<Individual> &population, int npop);

void simple_ga(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, int max_generation,
               double p_mute, double p_cross,int arc_index);

double start_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task, int arc_index);
double finish_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index);
double pe_ready(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index);

bool isFeasible(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &v,int arc_index);

double simple_ga_cost(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, Individual &v,
                int arc_index);

#endif //TASK_SCHED_SIMPLE_GA_H
