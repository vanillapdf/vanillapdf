include(${CMAKE_CURRENT_LIST_DIR}/minimum_version.cmake)
cmake_minimum_required(VERSION ${VANILLAPDF_MINIMUM_CMAKE_VERSION})

# Configure NuGet project templates. This script can be included from a regular
# build or executed directly via `cmake -P`. In both cases
# `CMAKE_CURRENT_SOURCE_DIR` resolves to the project root.

include(${CMAKE_CURRENT_LIST_DIR}/version.cmake)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/nuget/vanillapdf.runtime.csproj.in
    ${CMAKE_CURRENT_SOURCE_DIR}/nuget/vanillapdf.runtime.csproj
    @ONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/nuget/vanillapdf_net.targets.in
    ${CMAKE_CURRENT_SOURCE_DIR}/nuget/vanillapdf_net.targets
    @ONLY
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/nuget/vanillapdf.csproj.in
    ${CMAKE_CURRENT_SOURCE_DIR}/nuget/vanillapdf.csproj
    @ONLY
)
