# Set CDash build name for this build configuration


set(COMPILER_BITS "32-bit")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(COMPILER_BITS "64-bit")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)


set(BUILDNAME
    "${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR} ${CMAKE_CXX_COMPILER_ID}_${CMAKE_CXX_COMPILER_VERSION}_${COMPILER_BITS}"
    CACHE STRING "Build name variable for CDash"
)
