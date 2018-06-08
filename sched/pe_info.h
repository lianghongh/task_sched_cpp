//
// Created by lianghong on 2018/6/8.
//

#ifndef TASK_SCHED_PE_INFO_H
#define TASK_SCHED_PE_INFO_H

#include <queue>

#define PE_COUNT 8
#define VOLTAGE_LEVEL_COUNT 5

extern double voltage_level[PE_COUNT][VOLTAGE_LEVEL_COUNT];
extern double threshold_voltage[PE_COUNT];

extern std::queue<int> run_queue[PE_COUNT];

#endif //TASK_SCHED_PE_INFO_H
