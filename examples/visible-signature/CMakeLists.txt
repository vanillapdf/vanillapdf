cmake_minimum_required(VERSION 3.20)
project(VisibleSignature C)

find_package(vanillapdf CONFIG REQUIRED)

add_executable(visible-signature main.c)
target_link_libraries(visible-signature PRIVATE vanillapdf::vanillapdf)
