cmake_minimum_required(VERSION 3.10)

project(OOP_LAB7)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab7 main.cpp src/npc.cpp src/orc.cpp src/druid.cpp src/slaver.cpp)

target_include_directories(lab7 PUBLIC include)
