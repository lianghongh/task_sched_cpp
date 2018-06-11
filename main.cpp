#include<iostream>
#include "tgff/tgff_tools.h"
#include "ga/galib.h"

using namespace std;

int main() {

    string path="/home/lianghong/schedule/example.tgff";
    string path2 = "/Users/lianghong/Documents/tgff-3.6/test/test.tgff";
    vector<TaskGraph> g;
    vector<ArcDict> a;
    vector<PeDict> p;
    read_graph(g,path2);
    read_arc(a, path2);
    read_pe(p, path2);
//    for(auto i:g)
//    {
//        std::cout<<i;
//    }

    doHGA(g[0],p,a,200,100,0.5,0);


    return 0;
}
