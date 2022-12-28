# Set additional compiler flags

# Local compilation definitions
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELEASE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -DRELEASE")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DRELEASE")

set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -DRELEASE")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DRELEASE")

if(CMAKE_COMPILER_IS_GNUCXX)

	# Enable maximum warning level
	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	
	# GCC warns on pragma region directive, which is only IDE feature
	# It could be solved with #ifdef only for MSVC
	# All other pragmas are only for suppressing specific warnings on MSVC
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
	
	# Variables in constructors should be initialized in order
	# they were declared. I prefer initializing variables
	# in order they make sense and are declared as constructor parameters.
	# This should be harmless unless you take at least a little care.
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")

	# On Linux arm GCC the entire log is filled with notes:

	# parameter passing for argument of type 'std::_Rb_tree<vanillapdf::syntax::IndirectReferenceObject,
	# std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool>,
	# std::_Select1st<std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool> >,
	# std::less<vanillapdf::syntax::IndirectReferenceObject>,
	# std::allocator<std::pair<const vanillapdf::syntax::IndirectReferenceObject, bool> > >::const_iterator' changed in GCC 7.1

	# That warning is telling you that there was a subtle ABI change
	# (actually a conformance fix) between 6 and 7.1,
	# such that libraries built with 6.x or earlier may not work properly when called
	# from code built with 7.x (and vice-versa).
	# As long as all your C++ code is built with GCC 7.1 or later, you can safely ignore this warning.
	# To disable it, pass -Wno-psabi to the compiler.
	# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=77728
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")

endif(CMAKE_COMPILER_IS_GNUCXX)

if(WIN32)

	# Enable maximum warning level
	set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

	# We want to statically link most of the things on Windows
	# as it is hell to deploy multiple binaries alongside

	# UPDATE 2022:
	# Some dependencies such as libjpeg and OpenSSL do not support static CRT
	# This means that we have to ship CRL libraries in any case
	# Let's use this and reduce the libvanillapdf size by dynamically linking the CRT

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

	set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MT")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")

	set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MT")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT")

endif(WIN32)