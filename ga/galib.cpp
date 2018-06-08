//
// Created by lianghong on 2018/6/7.
//

#include <set>
#include "galib.h"
#include "../sched/task_graph.h"

std::uniform_int_distribution<int> task_u, pe_u, voltage_u;
std::uniform_real_distribution<double> real;

std::default_random_engine e;

Individual mutate_func1(Individual &);

Individual mutate_func2(Individual &);

Individual mutate_func3(Individual &);

Individual mutate_func4(Individual &);

std::vector<Mutate_func> func_list = {mutate_func1, mutate_func2, mutate_func3, mutate_func4};

void init(int task_size) {
    task_u = std::uniform_int_distribution<int>(0, task_size - 1);
    pe_u = std::uniform_int_distribution<int>(0, PE_COUNT - 1);
    voltage_u = std::uniform_int_distribution<int>(0, VOLTAGE_LEVEL_COUNT - 1);
    real = std::uniform_real_distribution<double>(0, 1);
    e.seed((unsigned) time(0));
}


void cross_over(Individual &v1, Individual &v2) {
    int cross_point1, cross_point2;
    do {
        cross_point1 = task_u(e);
        cross_point2 = task_u(e);
    } while (cross_point1 == cross_point2);

    if (cross_point1 > cross_point2) {
        int t = cross_point1;
        cross_point1 = cross_point2;
        cross_point2 = t;
    }
    for (int i = cross_point1; i <= cross_point2; i++) {
        Attribute b = v1.v[i];
        v1.v[i] = v2.v[i];
        v2.v[i] = b;
    }
}

Individual mutate_func1(Individual &p) {
    Individual v = p;
    int mutate = task_u(e);
    v.v[mutate].pe_index = pe_u(e);
    v.v[mutate].voltage_level = voltage_u(e);
    return v;
}

Individual mutate_func2(Individual &p) {
    Individual v = p;
    int p1, p2;
    do {
        p1 = task_u(e);
        p2 = task_u(e);
    } while (p1 == p2);
    if (p1 > p2) {
        int t = p1;
        p1 = p2;
        p2 = t;
    }
    Attribute b = v.v[p1];
    v.v[p1] = v.v[p2];
    v.v[p2] = b;
    return v;
}

Individual mutate_func3(Individual &p) {
    Individual v = p;
    int mutate = task_u(e);
    v.v[mutate].voltage_level = voltage_u(e);
    return v;
}

Individual mutate_func4(Individual &p) {
    Individual v = p;
    int mutate = task_u(e);
    v.v[mutate].pe_index = pe_u(e);
    return v;
}

Individual mutate(Individual &v) {
    std::uniform_int_distribution<int> u(0, func_list.size() - 1);
    return func_list[u(e)](v);
}

double cost(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, Individual &v, int arc_index) {
    double cost = 0;
    for (int i = 0; i < v.v.size(); i++) {
        double new_voltage = voltage_level[v.v[i].pe_index][v.v[i].voltage_level];
        double factor = new_voltage / voltage_level[v.v[i].pe_index][VOLTAGE_LEVEL_COUNT - 1];
        cost += pe_dict[v.v[i].pe_index].pe_dict[g.nodes[v.v[i].task_index].type] * pe_dict[v.v[i].pe_index].power *
                factor * factor;
        for (ArcNode *p = g.nodes[v.v[i].task_index].next; p != nullptr; p = p->next) {
            int arr = -1;
            for (int j = 0; j < v.v.size(); j++) {
                if (v.v[j].task_index == p->task_index) {
                    arr = j;
                    break;
                }
            }
            if (arr != -1) {
                cost += v.v[arr].pe_index != v.v[i].pe_index ? arc_dict[arc_index].power *
                                                               arc_dict[arc_index].arc_dict[p->type] : 0;
            } else {
                printf("can't find right node\n");
                exit(1);
            }
        }
    }
    return v.fitness = cost;
}


void init_population(TaskGraph &g, std::vector<Individual> &population, int npop) {
    int n = 0;
    while (n < npop) {
        int pos = 0;
        Individual individual;
        std::vector<TaskNode> task_set;
        std::queue<TaskNode> q;
        for (int i = 0; i < g.task_num; i++)
            task_set.push_back(g.nodes[i]);

        for (auto i = task_set.begin(); i != task_set.end(); i++) {
            if ((*i).in_degree == 0)
                q.push(*i);
        }

        while (!q.empty()) {
            TaskNode t = q.front();
            q.pop();
            individual.v[pos].task_index = t.task_index;
            individual.v[pos].pe_index = pe_u(e);
            individual.v[pos].voltage_level = voltage_u(e);
            for (ArcNode *p = t.next; p != nullptr; p = p->next)
            {
                if(task_set[p->task_index].in_degree--==0)
                    q.push(task_set[p->task_index]);
            }
            pos++;
        }
        if (isFeasible(individual)) {
            population.push_back(individual);
            n++;
        }
    }

}



bool isFeasible(Individual &v)
{

}

void doHGA(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, int max_generation,
           double p_mute) {
    std::vector<Individual> population;
    init(g.task_num);
    init_population(g, population, pop_size);

    for (int n = 0; population.size() > 1 && n < max_generation; n++) {
        for (int i = 0; i < population.size(); i += 2) {
            if (i + 1 < population.size())
                cross_over(population[i], population[i + 1]);
        }

        for (int i = 0; i < population.size(); i++) {
            if (!isFeasible(population[i]))
                population.erase(population.begin() + i);
        }

        for (int i = 0; i < population.size(); i++) {
            if (real(e) < p_mute) {
                int pp = 0;
                double costs = cost(g, pe_dict, arc_dict, population[i], 0);
                do {
                    Individual x = mutate(population[i]);
                    double new_cost = cost(g, pe_dict, arc_dict, x, 0);
                    if (new_cost < costs && isFeasible(x)) {
                        population[i] = x;
                        costs = new_cost;
                        pp -= max_generation;
                    } else
                        pp++;
                } while (pp <= max_generation);
            }
        }

    }
}


