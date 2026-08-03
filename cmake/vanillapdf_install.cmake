include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Skip CMake config file installation when an external package manager
# (e.g. Conan) generates its own find_package config files.
if(VANILLAPDF_SKIP_CMAKE_CONFIG_INSTALL)
  message(STATUS "Skipping CMake config file installation (VANILLAPDF_SKIP_CMAKE_CONFIG_INSTALL=ON)")
  return()
endif()

# Export target
install(EXPORT vanillapdfTargets
  FILE vanillapdfTargets.cmake
  NAMESPACE vanillapdf::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
  COMPONENT Development
)

# Generate and install config file
configure_package_config_file(
  ${CMAKE_CURRENT_LIST_DIR}/vanillapdfConfig.cmake.in
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
  COMPONENT Development
)

# Which dependencies a consumer has to resolve depends on how this package was
# built, so the build options are recorded in the generated file.
configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/vanillapdfDependencies.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfDependencies.cmake
  @ONLY
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfDependencies.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
  COMPONENT Development
)