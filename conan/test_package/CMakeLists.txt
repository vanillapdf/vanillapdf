cmake_minimum_required(VERSION 3.15)
project(test_vanillapdf LANGUAGES C CXX)

find_package(vanillapdf REQUIRED CONFIG)

add_executable(test_vanillapdf src/test.c)
target_link_libraries(test_vanillapdf PRIVATE vanillapdf::vanillapdf)

# vanillapdf is a C++ library with a C API. Static linking requires
# the C++ runtime, so use the C++ linker driver which handles this
# automatically across all compilers and platforms.
set_target_properties(test_vanillapdf PROPERTIES LINKER_LANGUAGE CXX)
