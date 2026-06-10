cmake_minimum_required(VERSION 3.15)

project(Chithra CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES
    "core/*.cpp"
)

add_library(chithra_core ${SOURCES})

target_include_directories(chithra_core
    PUBLIC
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/external
)

add_executable(Chithra
    tests/core/main.cpp
)

target_link_libraries(Chithra
    PRIVATE
    chithra_core
)