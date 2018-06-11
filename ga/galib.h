//
// Created by lianghong on 2018/6/7.
//

#ifndef TASK_SCHED_GALIB_H
#define TASK_SCHED_GALIB_H

#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#include <queue>
#include "../sched/pe_info.h"
#include "../tgff/tgff_tools.h"

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

void init(int);

void cross_over(Individual&,Individual&);

Individual mutate(Individual&);

double cost(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual& v,int arc_index);

void init_population(TaskGraph &g, std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,int arc_index,std::vector<Individual> &population, int npop);

bool isFeasible(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &v,int arc_index);



double start_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task, int arc_index);
double finish_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index);
double pe_ready(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index);

void init_runqueue(Individual &in);
void clear_runqueue(Individual& in);

void doHGA(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, int max_generation,
           double p_mute,double p_cross,int arc_index);

#endif //TASK_SCHED_GALIB_H
