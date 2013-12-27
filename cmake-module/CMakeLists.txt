project(bear:cmake-modules)

if( NOT BEAR_CMAKE_MODULES_INSTALL_PATH )
  set( BEAR_CMAKE_MODULES_INSTALL_PATH "share/cmake/bear-engine" )
endif( NOT BEAR_CMAKE_MODULES_INSTALL_PATH )

set( BEAR_CMAKE_MODULES
  bear-config.cmake
  )

#-------------------------------------------------------------------------------
# installation
install(
  FILES ${BEAR_CMAKE_MODULES}
  DESTINATION "${BEAR_CMAKE_MODULES_INSTALL_PATH}"
  )

#-------------------------------------------------------------------------------
# version file
set(
  BEAR_VERSION_FILE_NAME
  "${CMAKE_CURRENT_BINARY_DIR}/bear-config-version.cmake"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${BEAR_VERSION_FILE_NAME}"
  VERSION "${BEAR_VERSION}"
  COMPATIBILITY SameMajorVersion
  )

install(
  FILES "${BEAR_VERSION_FILE_NAME}"
  DESTINATION "${BEAR_CMAKE_MODULES_INSTALL_PATH}"
  )
