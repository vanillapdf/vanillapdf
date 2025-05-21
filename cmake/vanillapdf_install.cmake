include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install main target
install(TARGETS vanillapdf
  EXPORT vanillapdfTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Export target
install(EXPORT vanillapdfTargets
  FILE vanillapdfTargets.cmake
  NAMESPACE vanillapdf::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)

# Generate and install config file
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/vanillapdfConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)

# Generate and install version file
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/vanillapdfConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)

install(FILES
  ${PROJECT_SOURCE_DIR}/cmake/vanillapdfDependencies.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vanillapdf
)