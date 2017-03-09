# Require minimum CMAKE version
cmake_minimum_required(VERSION 2.6)

# Disable installing zlib dlls for now
set(SKIP_INSTALL_LIBRARIES true)

add_subdirectory(zlib)

# Organize the project structure
set_target_properties(zlib zlibstatic example minigzip PROPERTIES FOLDER "lib/zlib")

# Exclude zlib trash from building
set_target_properties(zlibstatic example minigzip PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

if(example64)
	set_target_properties(example64 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1 FOLDER "lib/zlib")
endif()

if(minigzip64)
	set_target_properties(minigzip64 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1 FOLDER "lib/zlib")
endif()