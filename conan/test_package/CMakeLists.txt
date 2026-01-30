cmake_minimum_required(VERSION 3.15)
project(test_vanillapdf LANGUAGES C CXX)

find_package(vanillapdf REQUIRED CONFIG)

add_executable(test_vanillapdf src/test.c)
target_link_libraries(test_vanillapdf PRIVATE vanillapdf::vanillapdf)
