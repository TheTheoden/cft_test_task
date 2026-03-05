cmake_minimum_required(VERSION 3.15)

project(cft C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include_directories(include)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(common STATIC src/common.c)
target_include_directories(common PUBLIC include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(process
    src/process.c
    src/common.c
)

add_executable(test
    src/test.c
    src/common.c
)