# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

# Disable installing zlib dlls for now
set(SKIP_INSTALL_LIBRARIES true)

add_subdirectory(zlib)

# Exclude zlib trash from building
set_target_properties(zlibstatic example minigzip example64 minigzip64 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)