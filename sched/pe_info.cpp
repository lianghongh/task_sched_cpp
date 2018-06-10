//
// Created by lianghong on 2018/6/8.
//

#include "pe_info.h"

double voltage_level[PE_COUNT][VOLTAGE_LEVEL_COUNT]={
        {0.8,1.2,1.4,2.0,3.3},
        {0.8,1.2,1.4,2.0,3.3},
        {0.8,1.2,1.4,2.0,3.3},
        {0.8,1.2,1.4,2.0,3.3},
        {0.8,1.2,1.4,2.0,3.3},
        {0.8,1.2,1.4,2.0,3.3},
        {0.8,1.2,1.4,2.0,3.3},
        {0.8,1.2,1.4,2.0,3.3}
};

double threshold_voltage[PE_COUNT]={
        0.5,
        0.5,
        0.5,
        0.5,
        0.5,
        0.5,
        0.5,
        0.5
};

std::vector<int> run_queue[PE_COUNT];

