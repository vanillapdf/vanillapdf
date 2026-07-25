cmake_minimum_required(VERSION 3.15)
project(test_package LANGUAGES C CXX)

find_package(vanillapdf REQUIRED CONFIG)

add_executable(${PROJECT_NAME} test_package.c)
target_link_libraries(${PROJECT_NAME} PRIVATE vanillapdf::vanillapdf)

# vanillapdf is a C++ library with a C API. Static linking requires
# the C++ runtime, so use the C++ linker driver which handles this
# automatically across all compilers and platforms.
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
