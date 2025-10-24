cmake_minimum_required(VERSION 3.30)
project(lab3)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

# Установка Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(lib src/point.cpp
                src/figure.cpp
                src/polygon.cpp
                src/square.cpp
                src/triangle.cpp
                src/octagon.cpp
)
target_include_directories(lib PUBLIC include)

add_executable(program main.cpp src/shapes.cpp)

target_link_libraries(program lib)

add_executable(tests_figure tests/unit_figure.cpp)
add_executable(tests_shapes tests/unit_shapes.cpp src/shapes.cpp)

target_link_libraries(tests_figure lib gtest gtest_main)
target_link_libraries(tests_shapes lib gtest gtest_main)

enable_testing()

add_test(NAME Tests1 COMMAND tests_figure)
add_test(NAME Tests2 COMMAND tests_shapes)
