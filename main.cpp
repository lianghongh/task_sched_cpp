#include<iostream>
#include "tgff/tgff_tools.h"
#include "ga/hga.h"

using namespace std;

int main() {

    string path="/home/lianghong/schedule/example.tgff";
    string mac_path = "/Users/lianghong/Documents/tgff-3.6/test/test.tgff";
    vector<TaskGraph> g;
    vector<ArcDict> arc_dict;
    vector<PeDict> pe_dict;
    read_graph(g,mac_path);
    read_arc(arc_dict, mac_path);
    read_pe(pe_dict, mac_path);

//    Individual in;
//    in.v.push_back({0,0,1,-1,-1});
//    in.v.push_back({1,2,2,-1,-1});
//    in.v.push_back({3, 7, 3,-1,-1});
//    in.v.push_back({2, 2, 3,-1,-1});
//    in.v.push_back({7, 4, 4,-1,-1});
//    in.v.push_back({5, 4, 2,-1,-1});
//    in.v.push_back({6, 1, 1,-1,-1});
//    in.v.push_back({4, 7, 2,-1,-1});
//
//    init_runqueue(in);
//    for(int i=0;i<in.v.size();i++)
//    {
//        cout <<in.v[i].task_index<<" finish_time: "<<finish_time(g[0], pe_dict, arc_dict, in, in.v[i].task_index,0)<<"\n";
//    }
//    clear_runqueue(in);

    doHGA(g[0], pe_dict, arc_dict, 60, 1000, 0.1, 0.8,10,0);


    return 0;
}
