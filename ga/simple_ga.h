//
// Created by lianghong on 18-6-19.
//

#ifndef TASK_SCHED_SIMPLE_GA_H
#define TASK_SCHED_SIMPLE_GA_H


#include <vector>
#include "ga_tools.h"
#include "../sched/task_graph.h"
#include "../tgff/tgff_tools.h"

int simple_ga_select(std::vector<Individual> &population);

void simple_ga_crossover(Individual &parent1, Individual &parent2);

Individual simple_ga_mutate(Individual &in);

void simple_ga(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, int max_generation,
               double p_mute, double p_cross,int arc_index);

void simple_ga(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, unsigned exec_time,
               double p_mute, double p_cross,int arc_index);

#endif //TASK_SCHED_SIMPLE_GA_H
