//
// Created by lianghong on 2018/6/7.
//

#ifndef TASK_SCHED_GALIB_H
#define TASK_SCHED_GALIB_H

#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include "../sched/pe_info.h"
#include "../tgff/tgff_tools.h"

typedef struct {
    int task_index;
    int pe_index;
    int voltage_level;
}Attribute;

typedef struct{
    std::vector<Attribute> v;
    double fitness;
}Individual;

typedef void (*Mutate_func)(Individual&);

extern std::uniform_int_distribution<int> task_u,pe_u,voltage_u;
extern std::default_random_engine e;

void init(Individual&);
void cross_over(Individual&,Individual&);
void mutate(Individual&);
double cost(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual& v);



#endif //TASK_SCHED_GALIB_H
