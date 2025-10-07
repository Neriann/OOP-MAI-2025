cmake_minimum_required(VERSION 3.30)
project(lab2)

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

add_library(lib src/money.cpp)
target_include_directories(lib PUBLIC include)

add_executable(program main.cpp)

target_link_libraries(program lib)

add_executable(run_tests tests/unit.cpp)

target_link_libraries(run_tests lib gtest gtest_main)

enable_testing()

add_test(NAME LabTests COMMAND run_tests)
