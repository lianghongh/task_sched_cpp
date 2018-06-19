cmake_minimum_required(VERSION 3.8)
project(task_sched)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp tgff/tgff_tools.h tgff/tgff_tools.cpp ga/hga.h ga/hga.cpp sched/task_graph.h sched/task_graph.cpp sched/pe_info.h sched/pe_info.cpp ga/ga_tools.h ga/ga_tools.cpp ga/simple_ga.h ga/simple_ga.cpp)
add_executable(task_sched ${SOURCE_FILES})