#include<iostream>
#include "tgff/tgff_tools.h"
#include "algo/simple_ga.h"

using namespace std;

int main() {

    string path="/home/lianghong/schedule/example.tgff";
    string mac_path = "/Users/lianghong/Documents/tgff-3.6/test/test.tgff";
    vector<TaskGraph> g;
    vector<ArcDict> arc_dict;
    vector<PeDict> pe_dict;
    read_graph(g,path);
    read_arc(arc_dict, path);
    read_pe(pe_dict, path);


    simple_ga(g[0], pe_dict, arc_dict, 50, 100, 0.1, 0.8, 0);
//    hga(g[0], pe_dict, arc_dict, 100, 100, 0.2, 0.6, 3, 0);
    return 0;
}
