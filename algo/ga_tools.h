//
// Created by lianghong on 18-6-19.
//

#ifndef TASK_SCHED_GA_TOOLS_H
#define TASK_SCHED_GA_TOOLS_H

#include <vector>
#include <random>
#include "../sched/task_graph.h"

typedef struct {

    int task_index;
    int pe_index;
    int voltage_level;
    double start_time;
    double finish_time;

}Attribute;

typedef struct{
    std::vector<Attribute> v;
    double fitness;

}Individual;


extern std::uniform_int_distribution<int> task_u,pe_u,voltage_u;
extern std::uniform_real_distribution<double > real;
extern std::default_random_engine e;

void init_random(int);
void show_individual(Individual &in);
int getPe(Individual& in,int task);
int getIndex(Individual &in,int task);

double start(TaskGraph &g, Individual &in, int task);
double finish(TaskGraph &g, Individual &in, int task);
double pe(TaskGraph &g, Individual &in, int task);
bool isFeasible(TaskGraph &g,Individual &v);
double power_cost(TaskGraph &g,Individual &v);


#endif //TASK_SCHED_GA_TOOLS_H
