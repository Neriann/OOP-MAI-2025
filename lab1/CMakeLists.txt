cmake_minimum_required(VERSION 3.10)
project(gtest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Добавление опций компиляции
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

# Создаем исполняемый файл программы
add_executable(program main.cpp src/replacer.cpp)

# Создаем исполняемый файл тестов
add_executable(run_tests tests/unit.cpp src/replacer.cpp)

# Линкуем тесты (gtest_main -> чтобы не писать в файле с тестами блок main)
target_link_libraries(run_tests gtest gtest_main)

# Добавление тестов
enable_testing()

# Добавление тестов в тестовый набор
add_test(NAME MyProjectTests COMMAND run_tests)