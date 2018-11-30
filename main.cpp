#include<iostream>
#include "algo/simple_ga.h"
#include "algo/cuckoo.h"
#include "padc/padc.h"

using namespace std;

#define RUNALL
//#define MAC

int main() {

    int node_size;
    char path[1000];
#ifdef RUNALL
    int node_list[]={30,50,100,200,300};
    for(int k=0;k<sizeof(node_list)/sizeof(int);k++)
    {
        node_size=node_list[k];
#else
        node_size=30;
#endif

#ifdef MAC
        sprintf(path,"/Users/lianghong/Desktop/GraduateData/research2/input/node_%d/test.tgff",node_size);
#else
        sprintf(path,"/home/lianghong/Desktop/GraduateData/research2/input/node_%d/test.tgff",node_size);
#endif

        TaskGraph g;

        g.build(path);
#ifdef MAC
        sprintf(path,"/Users/lianghong/Desktop/GraduateData/research2/result/node_%d/ga",node_size);
        simple_ga(g,50, 5000, path,0.2, 0.8);
        sprintf(path,"/Users/lianghong/Desktop/GraduateData/research2/result/node_%d/cuckoo",node_size);
        cuckoo_search(g,50,5000,path,0.25,0.03,0.04);
        sprintf(path,"/Users/lianghong/Desktop/GraduateData/research2/result/node_%d/PADC",node_size);
        PADC(g, 50, 5000, path, 0.8, 0.2);
#else

        sprintf(path,"/home/lianghong/Desktop/GraduateData/research2/result/node_%d/ga",node_size);
        simple_ga(g,50, 5000, path,0.2, 0.8);
        sprintf(path,"/home/lianghong/Desktop/GraduateData/research2/result/node_%d/cuckoo",node_size);
        cuckoo_search(g,50,5000,path,0.25,0.03,0.04);
        sprintf(path,"/home/lianghong/Desktop/GraduateData/research2/result/node_%d/PADC",node_size);
        PADC(g, 50, 5000, path, 0.8, 0.2);
#endif

        // save picture
        char command[100];
#ifdef MAC
        sprintf(command,"/Users/lianghong/CLionProjects/task_sched/draw_schedule_graph.py --path=/home/lianghong/Desktop/GraduateData/research2/result --node=%d",node_size);
#else
        sprintf(command,"/home/lianghong/CLionProjects/task_sched_cpp/draw_schedule_graph.py --path=/home/lianghong/Desktop/GraduateData/research2/result --node=%d",node_size);
#endif
        system(command);
#ifdef RUNALL
    }
#endif

    return 0;
}
