cmake_minimum_required(VERSION 3.15)

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

# The vanillapdf package ships the public headers, but it is packed without
# configuring the library, so the generated vanillapdf_config.h is produced here.
# It has to describe the natives in the runtime packages, which are all built from
# the presets with the default feature set. A function keeps these values from
# leaking into the build that includes this file.
function(vanillapdf_configure_nuget_config_header)
    set(VANILLAPDF_FEATURE_ENCRYPTION ON)
    set(VANILLAPDF_FEATURE_JPEG ON)
    set(VANILLAPDF_FEATURE_JPEG2000 ON)
    set(VANILLAPDF_FEATURE_ZLIB ON)

    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/vanillapdf/vanillapdf_config.h.in
        ${CMAKE_CURRENT_SOURCE_DIR}/nuget/include/vanillapdf/vanillapdf_config.h
    )
endfunction()

vanillapdf_configure_nuget_config_header()
