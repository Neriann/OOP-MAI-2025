cmake_minimum_required(VERSION 3.10)
project(gtest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Добавление опций компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

set(SOURCE_LIB src/replacer.cpp)

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

 # Создаем библиотеку, чтобы не компилировать SOURCE_LIB дважды
add_library(replacer_lib STATIC ${SOURCE_LIB})

# Подключаем к библиотеке хэдеры
target_include_directories(replacer_lib PUBLIC include)

# Создаем исполняемый файл программы
add_executable(program main.cpp)

target_link_libraries(program replacer_lib)

# Создаем исполняемый файл тестов
add_executable(run_tests tests/unit.cpp)

# Линкуем тесты (gtest_main -> чтобы не писать в файле с тестами блок main)
target_link_libraries(run_tests gtest gtest_main replacer_lib)

# Добавление тестов
enable_testing()

# Добавление тестов в тестовый набор
add_test(NAME LabTests COMMAND run_tests)