cmake_minimum_required(VERSION 3.20)
project(Signing C)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(signing main.c)
target_link_libraries(signing PRIVATE vanillapdf::vanillapdf)
