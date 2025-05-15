include(CMakePackageConfigHelpers)

install(EXPORT vanillapdfTargets
  FILE vanillapdfTargets.cmake
  NAMESPACE vanillapdf::
  DESTINATION lib/cmake/vanillapdf
)

configure_package_config_file(
  "${CMAKE_SOURCE_DIR}/cmake/vanillapdfConfig.cmake.in"
  "${CMAKE_BINARY_DIR}/vanillapdfConfig.cmake"
  INSTALL_DESTINATION lib/cmake/vanillapdf
)

write_basic_package_version_file(
  "${CMAKE_BINARY_DIR}/vanillapdfConfigVersion.cmake"
  VERSION ${VANILLAPDF_VERSION_MAJOR}.${VANILLAPDF_VERSION_MINOR}.${VANILLAPDF_VERSION_PATCH}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_BINARY_DIR}/vanillapdfConfig.cmake"
  "${CMAKE_BINARY_DIR}/vanillapdfConfigVersion.cmake"
  DESTINATION lib/cmake/vanillapdf
)