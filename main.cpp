#include<iostream>
#include "algo/simple_ga.h"
#include "algo/cuckoo.h"

using namespace std;

int main() {

    string path="/home/lianghong/schedule/example.tgff";
    string mac_path = "/Users/lianghong/Documents/tgff-3.6/test/test.tgff";


    TaskGraph g;
    g.build(path);
//    simple_ga(g,50, 100, 0.2, 0.8);
    cuckoo_search(g,50,1000,0.25,0.06);


    return 0;
}
