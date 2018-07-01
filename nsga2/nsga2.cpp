//
// Created by lianghong on 18-6-29.
//

#include <queue>
#include "nsga2.h"
#include <algorithm>
#include "../algo/ga_tools.h"

double max_object[OBJECT_COUNT]={INT32_MIN,INT32_MIN};
double min_object[OBJECT_COUNT]={INT32_MAX,INT32_MAX};

bool cmp_power(Individual &v1,Individual &v2)
{
    return v1.power<v2.power;
}

bool cmp_time(Individual &v1,Individual &v2)
{
    return v1.time<v2.time;
}

void cal_objective(TaskGraph &g,Individual &individual)
{
    power_cost(g, individual);
    time_cost(g, individual);
    if(individual.power<min_object[0])
        min_object[0]=individual.power;
    if(individual.power>max_object[0])
        max_object[0]=individual.power;
    if(individual.time<min_object[1])
        min_object[1]=individual.time;
    if(individual.time>max_object[1])
        max_object[1]=individual.time;
}


bool dominates(TaskGraph &g,Individual &in1, Individual &in2)
{
    bool worse = in1.time <= in2.time && in1.power <= in2.power;
    bool better = in1.time < in2.time || in1.power < in2.power;
    return worse&&better;
}

void generateIndividual(TaskGraph &g,Individual &individual)
{

    std::vector<TaskNode> task_set;
    std::queue<TaskNode> q;
    for (int i = 0; i < g.task_num; i++)
        task_set.push_back(g.nodes[i]);

    for (int i = 0; i<task_set.size(); i++) {
        if (task_set[i].in_degree == 0)
            q.push(task_set[i]);
    }

    while (!q.empty()) {
        TaskNode t = q.front();
        q.pop();
        Attribute aa;
        aa.task_index=t.task_index;
        aa.pe_index=pe_u(e);
        aa.voltage_level=voltage_u(e);
        aa.start_time=aa.finish_time=-1;
        individual.v.push_back(aa);
        for (ArcNode *p = t.next; p != nullptr; p = p->next)
        {
            if(--task_set[p->task_index].in_degree==0)
                q.push(task_set[p->task_index]);
        }
    }
    cal_objective(g, individual);
}

void cal_crowding_distance(std::vector<Individual> &front)
{
    if(front.size())
    {
        int solutions_num=front.size();
        for(int i=0;i<front.size();i++)
            front[i].crowding_distance=0;
        std::sort(front.begin(),front.end(),cmp_power);
        front[0].crowding_distance=max_object[0];
        front[solutions_num-1].crowding_distance=max_object[0];
        for(int i=1;i<solutions_num-1;i++)
        {
            front[i].crowding_distance+=(front[i+1].crowding_distance-front[i-1].crowding_distance)/(max_object[0]-min_object[0]);
        }
        std::sort(front.begin(),front.end(),cmp_time);
        front[0].crowding_distance=max_object[1];
        front[solutions_num-1].crowding_distance=max_object[1];
        for(int i=1;i<solutions_num-1;i++)
        {
            front[i].crowding_distance+=(front[i+1].crowding_distance-front[i-1].crowding_distance)/(max_object[1]-min_object[1]);
        }
    }
}

bool crowd_operator(Individual &in,Individual &other)
{
    return in.rank < other.rank || (in.rank == other.rank && in.crowding_distance > other.crowding_distance);
}

void fast_nondominate_sort(TaskGraph &g,Population &pop)
{
    pop.fronts.emplace_back();
    for(int i=0;i<pop.population.size();i++)
    {
        pop.population[i].dominate_count=0;
        pop.population[i].solutions.clear();
        for(int j=0;j<pop.population.size();j++)
        {
            if(dominates(g,pop.population[i],pop.population[j]))
                pop.population[i].solutions.push_back(pop.population[j]);
            else if(dominates(g,pop.population[j],pop.population[i]))
                pop.population[i].dominate_count++;
        }
        if(pop.population[i].dominate_count==0)
        {
            pop.population[i].rank=0;
            pop.fronts[0].push_back(pop.population[i]);
        }
    }
    for(int i=0;pop.fronts[i].size()>0;i++)
    {
        std::vector<Individual> temp;
        for(int j=0;j<pop.fronts[i].size();j++)
        {
            for(int k=0;k<pop.fronts[i][j].solutions.size();k++)
            {
                if(--pop.fronts[i][j].solutions[k].dominate_count==0)
                {
                    pop.fronts[i][j].solutions[k].rank=i+1;
                    temp.push_back(pop.fronts[i][j].solutions[k]);
                }
            }
        }
        pop.fronts.push_back(temp);
    }

}

void init_population(TaskGraph &g,Population &pop,int pop_size)
{
    for(int i=0;i<pop_size;i++)
    {
        Individual in;
        generateIndividual(g,in);
        pop.population.push_back(in);
    }
}

void create_children(TaskGraph &g,Population &parent,Population &child, double cp, double mp,int candidate)
{
    child.population.clear();
    child.fronts.clear();
    while (child.population.size()<parent.population.size())
    {
        int parent1,parent2;
        do {
            parent1 = tournament(parent, candidate);
            parent2 = tournament(parent, candidate);
        } while (parent1 == parent2);

        Individual pp1 = parent.population[parent1], pp2 = parent.population[parent2];
        if(cp<real(e))
            nsga_crossover(pp1, pp2);
        if(mp<real(e))
        {
            nsga_mutate(pp1);
            nsga_mutate(pp2);
        }
        cal_objective(g, pp1);
        cal_objective(g, pp2);
        child.population.push_back(pp1);
        child.population.push_back(pp2);
    }
}

void nsga_crossover(Individual &parent1, Individual &parent2)
{
    int cross_point1,cross_point2;
    do{
        cross_point1=task_u(e);
        cross_point2=task_u(e);
    }while (cross_point1==cross_point2);

    if(cross_point1>cross_point2)
    {
        int t=cross_point1;
        cross_point1=cross_point2;
        cross_point2=t;
    }
    for(int i=cross_point1;i<=cross_point2;i++)
    {
        Attribute b=parent1.v[i];
        parent1.v[i]=parent2.v[i];
        parent2.v[i]=b;
    }
}

void nsga_mutate(Individual &in)
{
    int mutate=task_u(e);
    in.v[mutate].pe_index=pe_u(e);
    in.v[mutate].voltage_level = voltage_u(e);
}

int tournament(Population &pop,int candidate)
{
    std::vector<int> cac;
    std::uniform_int_distribution<int> u(0,pop.population.size()-1);
    for(int i=0;i<candidate;i++)
        cac.push_back(u(e));
    int best_index=cac[0];
    Individual best = pop.population[best_index];
    for(int i=1;i<cac.size();i++)
    {
        if(crowd_operator(pop.population[cac[i]],best))
        {
            best=pop.population[cac[i]];
            best_index=cac[i];
        }
    }
    return best_index;
}



void NSGA2(TaskGraph &g,int pop_size, int max_generation,double cp, double mp)
{
    init_random(g.task_num);
    Population pop;
    init_population(g,pop,pop_size);
    fast_nondominate_sort(g,pop);
    for(int i=0;i<pop.fronts.size();i++)
        cal_crowding_distance(pop.fronts[i]);
    Population child;
    create_children(g, pop, child, cp, mp);
    for(int n=1;n<=max_generation;n++)
    {
        pop.population.insert(pop.population.end(),child.population.begin(),child.population.end());
        fast_nondominate_sort(g,pop);
        Population new_pop;
        int front_num = 0;
        while (new_pop.population.size()+pop.fronts[front_num].size()<=pop_size)
        {
            cal_crowding_distance(pop.fronts[front_num]);
            new_pop.population.insert(new_pop.population.end(),pop.fronts[front_num].begin(),pop.fronts[front_num].end());
            front_num++;
        }
        std::sort(pop.fronts[front_num].begin(),pop.fronts[front_num].end(),crowd_operator);
        new_pop.population.insert(new_pop.population.end(),pop.fronts[front_num].begin(),pop.fronts[front_num].begin()+pop_size-new_pop.population.size());
        std::cout<<"Gen "<<n<<": "<<pop<<"\n";
        pop=new_pop;
        create_children(g,pop,child,cp,mp);
    }
}
