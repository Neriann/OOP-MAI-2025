cmake_minimum_required(VERSION 3.10)

project(OOP_LAB6)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

add_library(entities_lib
        src/factory.cpp
        src/observer.cpp
        src/npc.cpp
        src/orc.cpp
        src/druid.cpp
        src/slaver.cpp
)

target_include_directories(entities_lib PUBLIC include)

add_executable(lab6 main.cpp)

target_link_libraries(lab6 entities_lib)

add_executable(tests tests/unit.cpp)

target_link_libraries(tests entities_lib gtest gtest_main)

enable_testing()

add_test(NAME Tests COMMAND tests_figure)
