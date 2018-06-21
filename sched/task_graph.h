//
// Created by lianghong on 2018/6/7.
//

#ifndef TASK_SCHED_TASK_GRAPH_H
#define TASK_SCHED_TASK_GRAPH_H

#include <iostream>
#include <vector>
#include <map>


struct ArcDict
{
    double power;
    std::map<int,double> arc_dict;

};

struct PeDict
{
    double power;
    std::map<int,double> pe_dict;
};

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

    TaskNode& operator=(const TaskNode& p)
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
    std::vector<PeDict> pe_dict;
    std::vector<ArcDict> arc_dict;
    int period;
    int task_num;
    int arc_num,arc_index;

    TaskGraph(int max=10000);
    void add_arc(int from,int to,int type);
    void build(const char* path);
    void build(std::string &path);

private:
    void read_graph(const char* path);
    void read_graph(const std::string& path);
    void read_arc(const char* path);
    void read_arc(const std::string &path);
    void read_pe(const char *path);
    void read_pe(const std::string &path);

    std::vector<std::string> split(std::string& s,char c);
};


std::ostream& operator<<(std::ostream &out,TaskGraph &g);
std::ostream& operator<<(std::ostream &out, TaskNode &p);
std::ostream& operator<<(std::ostream &out,ArcNode &a);

#endif //TASK_SCHED_TASK_GRAPH_H
