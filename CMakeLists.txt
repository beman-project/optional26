# CMakeLists.txt                                                    -*-CMake-*-
#
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.27)

project(beman_optional26 VERSION 0.0.0 LANGUAGES CXX)

# Includes
include(CTest)
include(FetchContent)

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

# Build the tests only if enabled via the CLI flag: BUILD_TESTING.
if(BUILD_TESTING)
    # Fetch GoogleTest
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG
            e39786088138f2749d64e9e90e0f9902daa77c40 # release-1.15.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_subdirectory(src/beman/optional26)
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

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

# Coverage
configure_file("cmake/gcovr.cfg.in" gcovr.cfg @ONLY)

add_custom_target(
    process_coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running gcovr to process coverage results"
    COMMAND mkdir -p coverage
    COMMAND gcovr --config gcovr.cfg .
)
