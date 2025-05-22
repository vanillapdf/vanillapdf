include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Export target
install(EXPORT vanillapdfTargets
  FILE vanillapdfTargets.cmake
  NAMESPACE vanillapdf::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)

# Generate and install config file
configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/cmake/vanillapdfConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)

# Generate and install version file
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfConfigVersion.cmake
  VERSION ${VANILLAPDF_VERSION_MAJOR}.${VANILLAPDF_VERSION_MINOR}.${VANILLAPDF_VERSION_PATCH}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)

install(FILES
  ${CMAKE_SOURCE_DIR}/cmake/vanillapdfConfigCommon.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)

install(FILES
  ${CMAKE_SOURCE_DIR}/cmake/vanillapdfDependencies.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)