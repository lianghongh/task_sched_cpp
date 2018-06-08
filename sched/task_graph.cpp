//
// Created by lianghong on 2018/6/7.
//
#include "task_graph.h"

std::ostream& operator<<(std::ostream &out,TaskGraph &g)
{
    out<<"\nTaskGraph->";
    out<<"[period: "<<g.period<<", task_num: "<<g.task_num<<", arc_num: "<<g.arc_num<<"]";
    for(int i=0;i<g.task_num;i++)
        out<<"\n           "<<g.nodes[i];
    return out << "]\n";

}


std::ostream& operator<<(std::ostream &out, TaskNode &p)
{
    out<<"[task: "<<p.task_index<<", in_degree: "<<p.in_degree<<", out_degree: "<<p.out_degree<<", deadline: "<<p.deadline<<", type: "<<p.type<<", arc_node: [";
    ArcNode *a=p.next;
    while (a!= nullptr)
    {
        out<<*a;
        a=a->next;
    }
    return out<<"]]";
}


std::ostream& operator<<(std::ostream &out,ArcNode &a)
{
    return out<<"(task: "<<a.task_index<<", type: "<<a.type<<")";
}