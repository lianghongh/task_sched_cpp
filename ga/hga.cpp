//
// Created by lianghong on 2018/6/7.
//

#include <queue>
#include "hga.h"

Individual mutate_func1(Individual &p);
Individual mutate_func2(Individual &p);
Individual mutate_func3(Individual &p);

std::vector<Individual (*)(Individual&)> funcs={mutate_func1,mutate_func2,mutate_func3};


int hga_select(std::vector<Individual> &population)
{
    double total=0;
    for(int i=0;i<population.size();i++)
        total+=population[i].fitness;
    int parent;
    double select_p=real(e),p=0;
    for(int i=0;i<population.size();i++)
    {
        p+=population[i].fitness/total;
        if(select_p<p)
        {
            parent=i;
            break;
        }
    }
    return parent;
}

void replace_bad(std::vector<Individual> &population, Individual &in)
{
    double min_fitness=INT32_MAX;
    int r=0;
    for(int i=0;i<population.size();i++)
    {
        if(population[i].fitness<min_fitness)
        {
            min_fitness=population[i].fitness;
            r=i;
        }
    }
    population[r]=in;
}

void hga_cross_over(Individual &parent1, Individual &parent2)
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

void pmutate(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,std::vector<Individual> &population,int arc_index,Individual &in, double p_mute,
             double reward, bool &has_mutate)
{
    if (real(e) < p_mute) {
        int pp = 0;
        double costs = in.fitness;
        do {
            Individual x = hga_mutate(in);
            hga_cost(g, pe_dict, arc_dict, x, arc_index);
            if (x.fitness < costs && isFeasible(g, pe_dict, arc_dict, x, arc_index)) {
                in = x;
                costs = x.fitness;
                has_mutate=true;
                pp -= reward;
            } else
                pp++;
        } while (pp <= reward);
    }
}

Individual mutate_func1(Individual &p)
{
    Individual v=p;
    int mutate = task_u(e);
    v.v[mutate].voltage_level = voltage_u(e);
    return v;
}

Individual mutate_func2(Individual &p)
{
    Individual v=p;
    int mutate = task_u(e);
    v.v[mutate].pe_index=pe_u(e);
    return v;
}

Individual mutate_func3(Individual &p)
{
    Individual v=p;
    int mutate = task_u(e);
    v.v[mutate].pe_index=pe_u(e);
    v.v[mutate].voltage_level=voltage_u(e);
    return v;
}


Individual hga_mutate(Individual &p) {
    std::uniform_int_distribution<int> u(0,funcs.size()-1);
    return funcs[u(e)](p);
}

double hga_cost(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, Individual &v,
                int arc_index) {
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


void hga_init_population(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int arc_index,
                         std::vector<Individual> &population, int npop) {
    int n = 0;
    while (n < npop) {
        Individual individual;
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
        if (isFeasible(g,pe_dict,arc_dict,individual,arc_index)) {
            hga_cost(g, pe_dict, arc_dict, individual, arc_index);
            population.push_back(individual);
            n++;
        }
    }

}


bool isFeasible(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &v,int arc_index=0)
{
    for(int i=0;i<v.v.size();i++)
    {
        run_queue[v.v[i].pe_index].push_back(v.v[i].task_index);
        v.v[i].start_time=v.v[i].finish_time=-1;
    }

    for(int i=0;i<v.v.size();i++)
    {
        if(v.v[i].finish_time==-1)
            v.v[i].finish_time=finish_time(g,pe_dict,arc_dict,v,v.v[i].task_index,arc_index);
        if(v.v[i].finish_time>g.nodes[v.v[i].task_index].deadline)
        {
            for(int j=0;j<PE_COUNT;j++)
                run_queue[j].clear();

            for(int j=0;j<v.v.size();j++)
                v.v[j].start_time=v.v[j].finish_time=-1;
            return false;
        }
    }
    for(int i=0;i<PE_COUNT;i++)
        run_queue[i].clear();
    for(int i=0;i<v.v.size();i++)
        v.v[i].start_time=v.v[i].finish_time=-1;
    return true;
}

void hga(TaskGraph &g, std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict, int pop_size, int max_generation,
         double p_mute, double p_cross, int reward, int arc_index) {
    double min_cost=INT32_MAX;
    Individual best;
    std::vector<Individual> population;
    init_random(g.task_num);
    hga_init_population(g, pe_dict, arc_dict, arc_index, population, pop_size);
    for (int i = 0; i < pop_size; i++)
    {
        if(population[i].fitness<min_cost)
        {
            min_cost=population[i].fitness;
            best=population[i];
        }
    }
    std::cout<<"generation 0\n";
    show_individual(best);
    std::cout << "cost: " << best.fitness << "\n";
    std::cout<<"\n---------------------------------------------------------------------------------------------------------------------\n";



    for (int n = 1; n <=max_generation; n++) {
        int parent1,parent2;
        do{
            parent1 = hga_select(population);
            parent2 = hga_select(population);
        }while (parent1==parent2);

        Individual child1=population[parent1],child2=population[parent2];
        if(real(e)<p_cross)
        {
            hga_cross_over(child1, child2);
            hga_cost(g, pe_dict, arc_dict, child1, arc_index);
            hga_cost(g, pe_dict, arc_dict, child2, arc_index);
            if(child1.fitness>population[parent1].fitness)
                child1 = population[parent1];
            if(child2.fitness>population[parent2].fitness)
                child2 = population[parent2];
        }
        bool has_mutate1=false,has_mutate2=false;
        pmutate(g, pe_dict, arc_dict, population, arc_index, child1, p_mute, reward,has_mutate1);
        pmutate(g, pe_dict, arc_dict, population, arc_index, child2, p_mute, reward,has_mutate2);

        if(has_mutate1||isFeasible(g,pe_dict,arc_dict,child1,arc_index))
            replace_bad(population, child1);
        if(has_mutate2||isFeasible(g,pe_dict,arc_dict,child2,arc_index))
            replace_bad(population, child2);

        for (int i = 0; i < population.size(); i++) {
            if(population[i].fitness<min_cost)
            {
                min_cost=population[i].fitness;
                best=population[i];
            }
        }


        std::cout<<"generation "<<n<<"\n";
        show_individual(best);
        std::cout << "cost: " << best.fitness << "\n";
        std::cout<<"---------------------------------------------------------------------------------------------------------------------\n\n";

    }
}



double finish_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index)
{
    int pe_index=0,v_level=0;
    for(int i=0;i<in.v.size();i++)
    {
        if(in.v[i].task_index==task)
        {
            pe_index=in.v[i].pe_index;
            v_level=in.v[i].voltage_level;
            break;
        }
    }
    double t_min=pe_dict[pe_index].pe_dict[g.nodes[task].type];
    double w=t_min*pow(voltage_level[pe_index][VOLTAGE_LEVEL_COUNT-1]-threshold_voltage[pe_index],2)*voltage_level[pe_index][v_level]/(pow(voltage_level[pe_index][v_level]-threshold_voltage[pe_index],2)*voltage_level[pe_index][VOLTAGE_LEVEL_COUNT-1]);
    if(in.v[task].start_time==-1)
        in.v[task].start_time=start_time(g,pe_dict,arc_dict,in,task,arc_index);
    return w+in.v[task].start_time;
}

double pe_ready(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task,int arc_index)
{
    int pe_index=getPe(in,task);
    if(run_queue[pe_index][0]==task)
        return 0;
    int last_task;
    for(last_task=0;run_queue[pe_index][last_task]!=task;last_task++);
    double max_com=0;
    for(ArcNode *p=g.nodes[run_queue[pe_index][last_task-1]].next;p!= nullptr;p=p->next)
    {
        double arc_cost=pe_index!=getPe(in,p->task_index)?arc_dict[arc_index].arc_dict[p->type]:0;
        if(arc_cost>max_com)
            max_com=arc_cost;
    }
    return max_com+finish_time(g,pe_dict,arc_dict,in,run_queue[pe_index][last_task-1],arc_index);
}


double start_time(TaskGraph &g,std::vector<PeDict> &pe_dict,std::vector<ArcDict> &arc_dict,Individual &in,int task, int arc_index)
{
    double max_ft=0;
    for(ArcNode *p=g.nodes[task].pre;p!= nullptr;p=p->next)
    {
        if(in.v[p->task_index].finish_time==-1)
            in.v[p->task_index].finish_time=finish_time(g,pe_dict,arc_dict,in,p->task_index,arc_index);
        double arrive_time=in.v[p->task_index].finish_time;
        if(getPe(in,task)!=getPe(in,p->task_index))
            arrive_time+=arc_dict[arc_index].arc_dict[p->type];
        if(arrive_time>max_ft)
            max_ft=arrive_time;
    }
    double pe_time = pe_ready(g, pe_dict, arc_dict, in, task, arc_index);
    return pe_time>max_ft?pe_time:max_ft;
}