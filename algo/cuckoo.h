//
// Created by lianghong on 18-6-20.
//

#ifndef TASK_SCHED_CUCKOO_H
#define TASK_SCHED_CUCKOO_H

#include "../sched/task_graph.h"
#include "ga_tools.h"


double cs_finish(TaskGraph &g,Individual &in,int task);
double cs_pe(TaskGraph &g,Individual &in,int task);
double cs_start(TaskGraph &g,Individual &in,int task);

void cs_init_nest(TaskGraph &g,std::vector<Individual> &nests, int npop);
double cs_cost(TaskGraph &g,Individual &v);
void cuckoo_search(TaskGraph &g,int pop_size,int max_generation,double pa);
bool cs_sched(TaskGraph &g,Individual &v);

int get_best_nest(TaskGraph &g,std::vector<Individual> &nest,std::vector<Individual> &new_nest,std::vector<double> &fitness);


#endif //TASK_SCHED_CUCKOO_H
