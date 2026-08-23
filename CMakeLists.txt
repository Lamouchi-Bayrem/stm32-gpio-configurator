cmake_minimum_required(VERSION 3.16)
project(STM32GPIOConfigurator)
set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
add_executable(STM32GPIOConfigurator
 src/main.cpp
 src/mainwindow.cpp
 include/mainwindow.h)
target_include_directories(STM32GPIOConfigurator PRIVATE include)
target_link_libraries(STM32GPIOConfigurator PRIVATE Qt6::Widgets)
