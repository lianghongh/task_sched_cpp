#include<iostream>
#include "algo/simple_ga.h"
#include "algo/cuckoo.h"
#include "nsga2/nsga2.h"

using namespace std;

int main() {

    char mac_path[1000];
    int node_size=500;
    sprintf(mac_path,"/Users/lianghong/Documents/paper_data/node_%d/test.tgff",node_size);


    TaskGraph g;

    g.build(mac_path);
    sprintf(mac_path,"/Users/lianghong/Documents/paper_data/node_%d/simple_ga",node_size);
    simple_ga(g,50, 5000, mac_path,0.2, 0.8);
    sprintf(mac_path,"/Users/lianghong/Documents/paper_data/node_%d/cuckoo",node_size);
    cuckoo_search(g,50,5000,mac_path,0.25,0.03,0.04);
    sprintf(mac_path,"/Users/lianghong/Documents/paper_data/node_%d/MOPGA",node_size);
    NSGA2(g,50,5000,mac_path,0.8,0.2);

    return 0;
}
