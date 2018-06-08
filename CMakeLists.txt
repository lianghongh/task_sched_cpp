cmake_minimum_required(VERSION 3.8)
project(task_sched)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp tgff/tgff_tools.h tgff/tgff_tools.cpp ga/galib.h ga/galib.cpp sched/task_graph.h sched/task_graph.cpp algo/hga.h algo/hga.cpp sched/pe_info.h sched/pe_info.cpp)
add_executable(task_sched ${SOURCE_FILES})