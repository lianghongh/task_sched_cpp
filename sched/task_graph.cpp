//
// Created by lianghong on 2018/6/7.
//
#include <fstream>
#include "task_graph.h"

std::vector<std::string> TaskGraph::split(std::string& s,char c)
{
    std::vector<std::string> result;
    int begin=0,end=0;
    while (begin<s.size()&&end<s.size())
    {
        while (begin<s.size()&&(s[begin]==c||s[begin]=='\n'||s[begin]=='\t'||s[begin]=='\r'))
            begin++;
        end=begin;
        while (end<s.size()&&s[end]!=c&&s[end]!='\n'&&s[end]!='\t'&&s[end]!='\r')
            end++;
        if(begin<s.size())
            result.push_back(s.substr(begin,end-begin));
        begin=end;
    }
    return result;
}

void TaskGraph::read_graph(const char* path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        exit(1);
    }
    std::string line;
    getline(f,line);
    while (line.find("@TASK")==-1)
        getline(f, line);
    getline(f,line);
    period=atoi(split(line,' ')[1].c_str());
    getline(f, line);
    getline(f, line);
    int index=0;
    while (line.size()!=0)
    {
        nodes[index].task_index=index;
        nodes[index].type=atoi(split(line,' ')[3].c_str());
        index++;
        getline(f,line);
    }
    getline(f,line);
    task_num=index;
    while (line.size()!=0)
    {
        std::vector<std::string> t=split(line,' ');
        add_arc(atoi(split(t[3], '_')[1].c_str()),atoi(split(t[5],'_')[1].c_str()),atoi(t[7].c_str()));
        getline(f, line);
    }
    getline(f, line);
    while (line.size()!=0&&line!="}")
    {
        std::vector<std::string> t=split(line,' ');
        nodes[atoi(split(t[3],'_')[1].c_str())].deadline=atoi(t[5].c_str());
        getline(f, line);
    }
    f.close();
}

void TaskGraph::read_arc(const char* path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        exit(1);
    }
    std::string tmp;
    getline(f, tmp);
    while (1)
    {
        while (tmp.find("@ARC")==-1&&tmp.find("@PE")==-1)
            getline(f, tmp);
        if (tmp.find("@PE")!=-1)
            break;
        ArcDict dict;
        getline(f,tmp);
        getline(f,tmp);
        dict.power=atof(tmp.c_str());
        for (int i = 0; i < 3; i++)
            getline(f, tmp);
        getline(f,tmp);
        while (tmp!="}")
        {
            std::vector<std::string> ss=split(tmp,' ');
            dict.arc_dict[atoi(ss.at(0).c_str())]=atof(ss.at(1).c_str());
            getline(f,tmp);
        }
        arc_dict.push_back(dict);
    }
    f.close();
}

void TaskGraph::read_arc(const std::string &path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        exit(1);
    }
    std::string tmp;
    getline(f, tmp);
    while (1)
    {
        while (tmp.find("@ARC")==-1&&tmp.find("@PE")==-1)
            getline(f, tmp);
        if (tmp.find("@PE")!=-1)
            break;
        ArcDict dict;
        getline(f,tmp);
        getline(f,tmp);
        dict.power=atof(tmp.c_str());
        for (int i = 0; i < 3; i++)
            getline(f, tmp);
        getline(f,tmp);
        while (tmp!="}")
        {
            std::vector<std::string> ss=split(tmp,' ');
            dict.arc_dict[atoi(ss.at(0).c_str())]=atof(ss.at(1).c_str());
            getline(f,tmp);
        }
        arc_dict.push_back(dict);
    }
    f.close();
}

void TaskGraph::read_pe(const char *path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        exit(1);
    }
    std::string tmp;
    getline(f,tmp);
    while (1)
    {
        while (tmp.find("@PE")==-1&&!f.eof())
            getline(f,tmp);
        if(f.eof())
            break;
        getline(f, tmp);
        getline(f, tmp);
        PeDict dict;
        dict.power=atof(tmp.c_str());
        for (int i = 0; i < 3; i++)
            getline(f, tmp);
        getline(f, tmp);
        while (tmp!="}")
        {
            std::vector<std::string> ss=split(tmp,' ');
            dict.pe_dict[atoi(ss.at(0).c_str())]=atof(ss.at(2).c_str());
            getline(f, tmp);
        }
        pe_dict.push_back(dict);
    }
    f.close();
}

void TaskGraph::read_pe(const std::string &path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        exit(1);
    }
    std::string tmp;
    getline(f,tmp);
    while (1)
    {
        while (tmp.find("@PE")==-1&&!f.eof())
            getline(f,tmp);
        if(f.eof())
            break;
        getline(f, tmp);
        getline(f, tmp);
        PeDict dict;
        dict.power=atof(tmp.c_str());
        for (int i = 0; i < 3; i++)
            getline(f, tmp);
        getline(f, tmp);
        while (tmp!="}")
        {
            std::vector<std::string> ss=split(tmp,' ');
            dict.pe_dict[atoi(ss.at(0).c_str())]=atof(ss.at(2).c_str());
            getline(f, tmp);
        }
        pe_dict.push_back(dict);
    }
    f.close();
}

void TaskGraph::read_graph(const std::string& path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        exit(1);
    }
    std::string line;
    getline(f,line);
    while (line.find("@TASK")==-1)
        getline(f, line);
    getline(f,line);
    period=atoi(split(line,' ')[1].c_str());
    getline(f, line);
    getline(f, line);
    int index=0;
    while (line.size()!=0)
    {
        nodes[index].task_index=index;
        nodes[index].type=atoi(split(line,' ')[3].c_str());
        index++;
        getline(f,line);
    }
    getline(f,line);
    task_num=index;
    while (line.size()!=0)
    {
        std::vector<std::string> t=split(line,' ');
        add_arc(atoi(split(t[3], '_')[1].c_str()),atoi(split(t[5],'_')[1].c_str()),atoi(t[7].c_str()));
        getline(f, line);
    }
    getline(f, line);
    while (line.size()!=0&&line!="}")
    {
        std::vector<std::string> t=split(line,' ');
        nodes[atoi(split(t[3],'_')[1].c_str())].deadline=atoi(t[5].c_str());
        getline(f, line);
    }
    f.close();
}



TaskGraph::TaskGraph(int max)
{
    nodes = std::vector<TaskNode>(max);
    period=-1;
    task_num=arc_num=0;
    arc_index=0;
}

void TaskGraph::add_arc(int from,int to,int type)
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

void TaskGraph::build(const char* path)
{
    read_graph(path);
    read_arc(path);
    read_pe(path);
}

void TaskGraph::build(std::string &path)
{
    read_graph(path);
    read_arc(path);
    read_pe(path);
}


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
    out<<"[task: "<<p.task_index<<", in_degree: "<<p.in_degree<<", out_degree: "<<p.out_degree<<", deadline: "<<p.deadline<<", type: "<<p.type<<", next_node: [";
    ArcNode *a=p.next;
    while (a)
    {
        out<<*a;
        a=a->next;
    }
    out<<"], pre_node: [";
    a=p.pre;
    while (a)
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