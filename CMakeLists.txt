# cmake-format: off
# /CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

cmake_minimum_required(VERSION 3.27)

project(
  beman_optional26
  VERSION 0.0.0
  LANGUAGES CXX)

cmake_policy(VERSION 3.27)

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

if("${CMAKE_BUILD_TESTS}")
  enable_testing()

  # Fetch GoogleTest
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
  )
  FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(src)
add_subdirectory(examples)

include(GNUInstallDirs)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake)

install(
  EXPORT ${TARGETS_EXPORT_NAME}
  NAMESPACE ${CMAKE_PROJECT_NAME}
  DESTINATION ${INSTALL_CONFIGDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
  "cmake/Config.cmake.in"
  ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR})

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${INSTALL_CONFIGDIR})
