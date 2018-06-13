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

    ArcNode()
    {
        task_index=type=-1;
        next= nullptr;
    }

    ArcNode(const ArcNode& a)
    {
        task_index=a.task_index;
        type=a.type;
        next=a.next;
    }

    ArcNode& operator=(const ArcNode& a)
    {
        task_index=a.task_index;
        type=a.type;
        next=a.next;
        return *this;
    }

};


class TaskNode
{

    friend std::ostream& operator<<(std::ostream &out, TaskNode &p);

public:

    int task_index;
    double deadline;
    int type;
    ArcNode *next,*pre;
    int in_degree,out_degree;

    TaskNode()
    {
        task_index=type=-1;
        deadline=INT32_MAX;
        next= nullptr;
        pre= nullptr;
        in_degree=out_degree=0;
    }

    TaskNode(const TaskNode& p)
    {
        task_index=p.task_index;
        deadline=p.deadline;
        type=p.type;
        next=p.next;
        pre=p.pre;
        in_degree=p.in_degree;
        out_degree=p.out_degree;
    }

    TaskNode&operator=(const TaskNode& p)
    {
        task_index=p.task_index;
        deadline=p.deadline;
        type=p.type;
        next=p.next;
        pre=p.pre;
        in_degree=p.in_degree;
        out_degree=p.out_degree;
        return *this;
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

    TaskGraph(int max=10000)
    {
        nodes = std::vector<TaskNode>(max);
        period=-1;
        task_num=arc_num=0;
    }

    void add_arc(int from,int to,int type)
    {
        ArcNode *p1=new ArcNode();
        ArcNode *p2 = new ArcNode();
        p1->task_index=to;
        p2->task_index=from;
        p1->type=type;
        p2->type=type;
        p1->next=nodes[from].next;
        p2->next=nodes[to].pre;
        nodes[from].next=p1;
        nodes[to].pre=p2;
        arc_num++;
        nodes[from].out_degree++;
        nodes[to].in_degree++;
    }

};


std::ostream& operator<<(std::ostream &out,TaskGraph &g);
std::ostream& operator<<(std::ostream &out, TaskNode &p);
std::ostream& operator<<(std::ostream &out,ArcNode &a);

#endif //TASK_SCHED_TASK_GRAPH_H
