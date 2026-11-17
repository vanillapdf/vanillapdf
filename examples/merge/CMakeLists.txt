cmake_minimum_required(VERSION 3.20)
project(Merge C)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(merge main.c)
target_link_libraries(merge PRIVATE vanillapdf::vanillapdf)
