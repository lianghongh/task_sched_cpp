#include<iostream>
#include "tgff/tgff_tools.h"
#include "ga/galib.h"

using namespace std;

int main() {

    string path="/home/lianghong/schedule/example.tgff";
    std::vector<TaskGraph> g;
    std::vector<ArcDict> a;
    std::vector<PeDict> p;
    read_graph(g,path);
    read_arc(a, path);
    read_pe(p, path);
//    for(auto i:g)
//    {
//        std::cout<<i;
//    }

    doHGA(g[0],p,a,100,50,0.5,0);
    return 0;
}
