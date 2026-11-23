cmake_minimum_required(VERSION 3.20)
project(Encryption C CXX)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(encryption main.c)
target_link_libraries(encryption PRIVATE vanillapdf::vanillapdf)
