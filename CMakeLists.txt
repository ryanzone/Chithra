cmake_minimum_required(VERSION 3.16)

project(Chithra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.10.3/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE CORE_SOURCES
    "core/*.cpp"
)

add_library(chithra_core ${CORE_SOURCES})

target_include_directories(chithra_core
    PUBLIC
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/external
)

add_executable(Chithra
    ui/main.cpp
    ui/resources/icons.qrc
    ui/MainWindow.cpp
    ui/MainWindow.hpp
    core/tools/ToolManager.cpp
    ui/CanvasWidget.cpp
    ui/CanvasWidget.hpp
    ui/LayerItemWidget.cpp
    ui/LayerItemWidget.hpp
    ui/Paneldock.cpp
    ui/Paneldock.hpp
)

target_include_directories(Chithra PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(Chithra
    PRIVATE
    chithra_core
    Qt6::Widgets
    Qt6::Svg
)