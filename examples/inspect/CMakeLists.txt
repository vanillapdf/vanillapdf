cmake_minimum_required(VERSION 3.20)
project(Inspect C)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(inspect main.c)
target_link_libraries(inspect PRIVATE vanillapdf::vanillapdf)
