cmake_minimum_required(VERSION 3.16)
project(GroupedTableHeader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(grouped_table_header
    main.cpp
    groupedheader.h
    groupedheader.cpp
)

target_link_libraries(grouped_table_header PRIVATE Qt6::Widgets)
