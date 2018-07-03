//
// Created by lianghong on 18-6-29.
//

#ifndef TASK_SCHED_NSGA2_H
#define TASK_SCHED_NSGA2_H


#include <vector>
#include "../algo/ga_tools.h"
#define OBJECT_COUNT 2


class Population{

    friend std::ostream& operator<<(std::ostream &out,Population &pop)
    {
        out<<"[";
        int i;
        for(i=0;i<pop.fronts[0].size()-1;i++)
            out<<*pop.fronts[0][i]<<" ";
        return out<<*pop.fronts[0][i]<<"]\n";
    }

public:
    std::vector<Individual> population;
    std::vector<std::vector<Individual*> > fronts;
};

extern double max_object[OBJECT_COUNT],min_object[OBJECT_COUNT];

void generateIndividual(TaskGraph &g,Individual &individual);

void cal_objective(TaskGraph &g,Individual &individual);

bool dominates(TaskGraph &g,Individual &in1,Individual &in2);

void fast_nondominate_sort(TaskGraph &g,Population &pop);

void nsga_crossover(Individual &parent1, Individual &parent2);

void nsga_mutate(Individual &in);

int tournament(Population &pop, int candidate);

void create_children(TaskGraph &g,Population &parent,Population &child, double cp, double mp,int candidate=3);

bool crowd_operator(Individual *in,Individual *other);

void cal_crowding_distance(std::vector<Individual*> &front);

void init_population(TaskGraph &g,Population &pop,int pop_size);

void NSGA2(TaskGraph &g,int pop_size, int max_generation,double cp, double mp);

#endif //TASK_SCHED_NSGA2_H
