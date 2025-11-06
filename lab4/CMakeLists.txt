cmake_minimum_required(VERSION 3.30)
project(OOP_LAB4)

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

include_directories(include)

add_executable(program main.cpp
        include/rectangle.hpp
        include/triangle.hpp
        include/square.hpp
        include/figure.hpp
        include/figure_array.hpp
)

add_executable(test_figures tests/test_figures.cpp
        include/rectangle.hpp
        include/triangle.hpp
        include/square.hpp
        include/figure.hpp
)

add_executable(test_array tests/test_array.cpp
        include/rectangle.hpp
        include/triangle.hpp
        include/square.hpp
        include/figure.hpp
)

target_link_libraries(test_figures gtest gtest_main)

target_link_libraries(test_array gtest gtest_main)

enable_testing()

add_test(NAME TestsFigure COMMAND test_figures)
add_test(NAME TestsArray COMMAND test_array)
