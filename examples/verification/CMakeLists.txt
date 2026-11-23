cmake_minimum_required(VERSION 3.20)
project(Verification C CXX)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(verification main.c)
target_link_libraries(verification PRIVATE vanillapdf::vanillapdf)
