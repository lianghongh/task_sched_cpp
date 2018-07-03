#include<iostream>
#include "algo/simple_ga.h"
#include "algo/cuckoo.h"
#include "nsga2/nsga2.h"

using namespace std;

int main() {

    string path="/home/lianghong/schedule/example.tgff";
    string mac_path = "/Users/lianghong/Documents/tgff-3.6/test/test.tgff";


    TaskGraph g;
    g.build(path);
//    cout << g << "\n";
//    simple_ga(g,50, 100, 0.2, 0.8);
    cuckoo_search(g,50,300,0.25,0.03);
//    NSGA2(g,50,300,0.8,0.1);
//    cout << g << "\n";

    return 0;
}
