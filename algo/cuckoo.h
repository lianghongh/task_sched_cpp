//
// Created by lianghong on 18-6-20.
//

#ifndef TASK_SCHED_CUCKOO_H
#define TASK_SCHED_CUCKOO_H

#include "../sched/task_graph.h"
#include "ga_tools.h"

void cs_init_nest(TaskGraph &g,std::vector<Individual> &nests, int npop);
void cuckoo_search(TaskGraph &g,int pop_size,int max_generation,double pa, double alpha=0.01,double beta=1.5);

#endif //TASK_SCHED_CUCKOO_H
