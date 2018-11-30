cmake_minimum_required(VERSION 3.8)
project(task_sched)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp sched/task_graph.h sched/task_graph.cpp sched/pe_info.h sched/pe_info.cpp algo/ga_tools.h algo/ga_tools.cpp algo/simple_ga.h algo/simple_ga.cpp algo/cuckoo.h algo/cuckoo.cpp padc/padc.h padc/padc.cpp)
add_executable(task_sched ${SOURCE_FILES})