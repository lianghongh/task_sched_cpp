//
// Created by lianghong on 2018/6/7.
//

#ifndef TASK_SCHED_TASK_GRAPH_H
#define TASK_SCHED_TASK_GRAPH_H

#include <iostream>
#include <vector>


class ArcNode
{

    friend std::ostream& operator<<(std::ostream &out,ArcNode& a);
public:
    int task_index;
    ArcNode *next;
    int type;
    double com_time;

    ArcNode()
    {
        task_index=type=-1;
        next= nullptr;
        com_time=0;
    }

    ArcNode(const ArcNode& a)
    {
        task_index=a.task_index;
        type=a.type;
        next=a.next;
        com_time=a.com_time;
    }

    ArcNode& operator=(const ArcNode& a)
    {
        task_index=a.task_index;
        type=a.type;
        next=a.next;
        com_time=a.com_time;
    }

};


class TaskNode
{

    friend std::ostream& operator<<(std::ostream &out, TaskNode &p);

public:

    int task_index;
    double deadline;
    int type;
    double exec_time;
    ArcNode *next;
    int in_degree,out_degree;

    TaskNode()
    {
        task_index=type=-1;
        deadline=0;
        exec_time=0;
        next= nullptr;
        in_degree=out_degree=0;
    }

    TaskNode(const TaskNode& p)
    {
        task_index=p.task_index;
        deadline=p.deadline;
        type=p.type;
        exec_time=p.exec_time;
        next=p.next;
        in_degree=p.in_degree;
        out_degree=p.out_degree;
    }

    TaskNode&operator=(const TaskNode& p)
    {
        task_index=p.task_index;
        deadline=p.deadline;
        type=p.type;
        exec_time=p.exec_time;
        next=p.next;
        in_degree=p.in_degree;
        out_degree=p.out_degree;
    }

};


class TaskGraph
{
    friend std::ostream& operator<<(std::ostream &out,TaskGraph &g);

public:

    std::vector<TaskNode> nodes;
    int period;
    int task_num;
    int arc_num;

    TaskGraph(int max=100)
    {
        nodes = std::vector<TaskNode>(max);
        period=-1;
        task_num=arc_num=0;
    }

    void add_arc(int from,int to,int type)
    {
        ArcNode *p=new ArcNode();
        p->task_index=to;
        p->type=type;
        p->next=nodes[from].next;
        nodes[from].next=p;
        arc_num++;
        nodes[from].out_degree++;
        nodes[to].in_degree++;
    }
};


std::ostream& operator<<(std::ostream &out,TaskGraph &g);
std::ostream& operator<<(std::ostream &out, TaskNode &p);
std::ostream& operator<<(std::ostream &out,ArcNode &a);

#endif //TASK_SCHED_TASK_GRAPH_H
