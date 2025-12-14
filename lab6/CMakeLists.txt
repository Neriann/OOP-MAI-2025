cmake_minimum_required(VERSION 3.10)

project(OOP_LAB6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab6 main.cpp src/npc.cpp src/orc.cpp src/druid.cpp src/slaver.cpp)

target_include_directories(lab6 PUBLIC include)
