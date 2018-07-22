#include<iostream>
#include "algo/simple_ga.h"
#include "algo/cuckoo.h"
#include "nsga2/nsga2.h"

using namespace std;

int main() {

    string path="/home/lianghong/schedule/example.tgff";
    string mac_path = "/Users/lianghong/Documents/sched_data/node_300/test.tgff";


    TaskGraph g;
    g.build(mac_path);
//    cout << g << "\n";
    simple_ga(g,50, 2000, string("/Users/lianghong/Documents/sched_data/node_300/simple_ga"),0.2, 0.8);
    cuckoo_search(g,50,2000,string("/Users/lianghong/Documents/sched_data/node_300/cuckoo"),0.25,0.03,0.04);
    NSGA2(g,50,2000,string("/Users/lianghong/Documents/sched_data/node_300/MOPGA"),0.8,0.2);

    return 0;
}
