#include<iostream>
#include "tgff/tgff_tools.h"

using std::string;

int main() {

    string path="/Users/lianghong/Documents/tgff-3.6/test/test.tgff";
    std::vector<TaskGraph> g;
    std::vector<ArcDict> a;
    std::vector<PeDict> p;
    read_graph(g,path);
    read_arc(a, path);
    read_pe(p, path);
    for(auto i:g)
    {
        std::cout<<i;
    }
    build(g[0],a[0],p[0]);
    std::cout<<g[0];
    return 0;
}
