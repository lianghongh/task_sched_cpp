//
// Created by lianghong on 2018/6/7.
//

#ifndef TASK_SCHED_TGFF_TOOLS_H
#define TASK_SCHED_TGFF_TOOLS_H

#include <map>
#include <string>
#include <vector>
#include "../sched/task_graph.h"

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


void read_arc(std::vector<ArcDict> &v,const char* path);
void read_arc(std::vector<ArcDict> &v,const std::string &path);

void read_pe(std::vector<PeDict> &v,const char *path);
void read_pe(std::vector<PeDict> &v,const std::string &path);

void read_graph(std::vector<TaskGraph> &v, const char* path);
void read_graph(std::vector<TaskGraph> &v, const std::string& path);

void build(TaskGraph&,ArcDict&,PeDict&);

std::vector<std::string> split(std::string& s,char c);


#endif //TASK_SCHED_TGFF_TOOLS_H
