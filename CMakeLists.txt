cmake_minimum_required(VERSION 3.10)

project(beman_optional26 VERSION 0.0.0 LANGUAGES CXX)

enable_testing()

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(examples)

include(GNUInstallDirs)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

install(
  EXPORT ${TARGETS_EXPORT_NAME}
  NAMESPACE ${CMAKE_PROJECT_NAME}
  DESTINATION ${INSTALL_CONFIGDIR}
  )

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "cmake/Config.cmake.in"
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)
