cmake_minimum_required(VERSION 3.8)
project(task_sched)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp tgff/tgff_tools.h tgff/tgff_tools.cpp ga/galib.h ga/galib.cpp graph/task_graph.h graph/task_graph.cpp)
add_executable(task_sched ${SOURCE_FILES})