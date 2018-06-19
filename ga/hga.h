//
// Created by lianghong on 2018/6/7.
//

#ifndef TASK_SCHED_GALIB_H
#define TASK_SCHED_GALIB_H

#include <cstdlib>
#include <vector>
#include "../sched/pe_info.h"
#include "../tgff/tgff_tools.h"
#include "ga_tools.h"



int hga_select(std::vector<Individual> &population);
void hga_cross_over(Individual &parent1, Individual &parent2);
void pmutate(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,std::vector<Individual> &population,int arc_index,Individual &in, double p_mute,
             double reward, bool &has_mutate);
void replace_bad(std::vector<Individual> &population, Individual &in);


Individual hga_mutate(Individual &p);

double hga_cost(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, Individual &v,
                int arc_index);

void hga_init_population(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int arc_index,
                         std::vector<Individual> &population, int npop);

bool isFeasible(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &v,int arc_index);



double start_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task, int arc_index);
double finish_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index);
double pe_ready(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index);


void hga(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, int max_generation,
         double p_mute, double p_cross, int reward, int arc_index);

#endif //TASK_SCHED_GALIB_H
