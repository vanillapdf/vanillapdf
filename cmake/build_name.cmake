# Set CDash build name for this build configuration

set(BUILDNAME
	"${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR} ${CMAKE_CXX_COMPILER_ID}_${CMAKE_CXX_COMPILER_VERSION}"
	CACHE STRING "Build name variable for CDash"
)
