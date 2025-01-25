cmake_minimum_required(VERSION 3.27)
project(gierka_na_interfejsy)

set(CMAKE_CXX_STANDARD 17)
add_executable(gierka_na_interfejsy main.cpp)

target_link_libraries(gierka_na_interfejsy setupapi hid)
