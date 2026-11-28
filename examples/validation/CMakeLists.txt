cmake_minimum_required(VERSION 3.20)
project(Validation C CXX)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(validation main.c)
target_link_libraries(validation PRIVATE vanillapdf::vanillapdf)
