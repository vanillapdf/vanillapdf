cmake_minimum_required(VERSION 3.20)
project(QuickStart C CXX)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(quickstart main.c)
target_link_libraries(quickstart PRIVATE vanillapdf::vanillapdf)
