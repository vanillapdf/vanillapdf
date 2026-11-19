cmake_minimum_required(VERSION 3.20)
project(Create C)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(create main.c)
target_link_libraries(create PRIVATE vanillapdf::vanillapdf)
