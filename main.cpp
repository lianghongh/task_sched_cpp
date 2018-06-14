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
    read_graph(g,path);
    read_arc(arc_dict, path);
    read_pe(pe_dict, path);


    Individual in;
    in.v.push_back({0, 2, 3, -1, -1});
    in.v.push_back({3, 2, 1, -1, -1});
    in.v.push_back({1, 4, 4, -1, -1});
    in.v.push_back({6, 4, 0, -1, -1});
    in.v.push_back({5, 1, 3, -1, -1});
    in.v.push_back({2, 0, 2, -1, -1});
    in.v.push_back({4, 6, 4, -1, -1});

    for (int i = 0; i < in.v.size(); i++)
        run_queue[in.v[i].pe_index].push_back(in.v[i].task_index);
    for(int i=0;i<in.v.size();i++)
    {
        cout << "task "<<i<<" start_time: " << start_time(g[0], pe_dict, arc_dict, in, i, 0) << " finish_time: "
             << finish_time(g[0], pe_dict, arc_dict, in, i, 0)<<"\n";
    }

    cout << "cost: " << cost(g[0], pe_dict, arc_dict, in, 0)<<"\n";




//    doHGA(g[0], pe_dict, arc_dict, 50, 1000, 0.1, 0.8,3,0);



    return 0;
}
