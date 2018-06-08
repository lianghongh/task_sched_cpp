//
// Created by lianghong on 2018/6/7.
//

#include "tgff_tools.h"
#include <iostream>
#include <fstream>
#include <string>

std::vector<std::string> split(std::string& s,char c)
{
    std::vector<std::string> result;
    int begin=0,end=0;
    while (begin<s.size()&&end<s.size())
    {
        while (begin<s.size()&&(s.at(begin)==c||s.at(begin)=='\n'||s[begin]=='\t'||s[begin]=='\r'))
            begin++;
        end=begin;
        while (end<s.size()&&s.at(end)!=c&&s.at(end)!='\n'&&s[end]!='\t'&&s[end]!='\r')
            end++;
        if(begin<s.size())
            result.push_back(s.substr(begin,end-begin));
        begin=end;
    }
    return result;
}

void read_arc(std::vector<ArcDict> &v,const std::string &path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        return;
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
        v.push_back(dict);
    }
    f.close();
}


void read_arc(std::vector<ArcDict> &v,const char* path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        return;
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
        v.push_back(dict);
    }
    f.close();
}

void read_pe(std::vector<PeDict> &v,const char *path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        return;
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
        v.push_back(dict);
    }
    f.close();
}
void read_pe(std::vector<PeDict> &v,const std::string &path)
{
    std::fstream f(path,std::ios::in);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        return;
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
        v.push_back(dict);
    }
    f.close();
}

void read_graph(std::vector<TaskGraph> &v, const char* path)
{
    std::fstream f(path);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        return;
    }
    std::string line;
    getline(f,line);
    while (1)
    {
        while (line.find("@TASK")==-1&&line.find("@ARC")==-1)
            getline(f, line);
        if(line.find("@ARC")!=-1)
            break;
        TaskGraph g;
        getline(f,line);
        g.period=atoi(split(line,' ')[1].c_str());
        getline(f, line);
        getline(f, line);
        int index=0;
        while (line.size()!=0)
        {
            g.nodes[index].task_index=index;
            g.nodes[index].type=atoi(split(line,' ')[3].c_str());
            index++;
            getline(f,line);
        }
        getline(f,line);
        g.task_num=index;
        while (line.size()!=0)
        {
            std::vector<std::string> t=split(line,' ');
            g.add_arc(atoi(split(t[3], '_')[1].c_str()),atoi(split(t[5],'_')[1].c_str()),atoi(t[7].c_str()));
            getline(f, line);
        }

        getline(f, line);
        while (line.size()!=0&&line!="}")
        {
            std::vector<std::string> t=split(line,' ');
            g.nodes[atoi(split(t[3],'_')[1].c_str())].deadline=atoi(t[5].c_str());
            getline(f, line);
        }
        v.push_back(g);
    }
    f.close();
}

void read_graph(std::vector<TaskGraph> &v, const std::string& path)
{
    std::fstream f(path);
    if(!f.is_open())
    {
        std::cout<<"can't open file->"<<path<<"\n";
        return;
    }
    std::string line;
    getline(f,line);
    while (1)
    {
        while (line.find("@TASK")==-1&&line.find("@ARC")==-1)
            getline(f, line);
        if(line.find("@ARC")!=-1)
            break;
        TaskGraph g;
        getline(f,line);
        g.period=atoi(split(line,' ')[1].c_str());
        getline(f, line);
        getline(f, line);
        int index=0;
        while (line.size()!=0)
        {
            g.nodes[index].task_index=index;
            g.nodes[index].type=atoi(split(line,' ')[3].c_str());
            index++;
            getline(f,line);
        }
        getline(f,line);
        g.task_num=index;
        while (line.size()!=0)
        {
            std::vector<std::string> t=split(line,' ');
            g.add_arc(atoi(split(t[3], '_')[1].c_str()),atoi(split(t[5],'_')[1].c_str()),atoi(t[7].c_str()));
            getline(f, line);
        }

        getline(f, line);
        while (line.size()!=0&&line!="}")
        {
            std::vector<std::string> t=split(line,' ');
            g.nodes[atoi(split(t[3],'_')[1].c_str())].deadline=atoi(t[5].c_str());
            getline(f, line);
        }
        v.push_back(g);
    }
    f.close();
}

