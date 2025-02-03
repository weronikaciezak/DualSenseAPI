cmake_minimum_required(VERSION 3.27)
project(DualSenseAPI)

set(CMAKE_CXX_STANDARD 17)
add_executable(DualSenseAPI main.cpp)

target_link_libraries(DualSenseAPI setupapi hid)
