//
// Created by lianghong on 18-6-20.
//

#ifndef TASK_SCHED_CUCKOO_H
#define TASK_SCHED_CUCKOO_H

#include "../sched/task_graph.h"
#include "../tgff/tgff_tools.h"

void cuckoo_search(TaskGraph &g,std::vector<PeDict> &pe_dict, std::vector<ArcDict> &arc_dict,int pop_size,int max_generation,double pa,int arc_index);

#endif //TASK_SCHED_CUCKOO_H
