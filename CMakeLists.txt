# CMakeLists.txt                                                    -*-CMake-*-
#
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.27...3.31)

project(beman_optional26 VERSION 0.0.0 LANGUAGES CXX)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed!")
endif()

option(
    OPTIONAL26_ENABLE_TESTING
    "Enable building tests and test infrastructure"
    ${PROJECT_IS_TOP_LEVEL}
)

# Includes
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(TARGET_PACKAGE_NAME ${PROJECT_NAME})
set(TARGETS_EXPORT_NAME ${TARGET_PACKAGE_NAME}-targets)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${TARGET_PACKAGE_NAME})

# Build the tests if enabled via the option OPTIONAL26_ENABLE_TESTING
if(OPTIONAL26_ENABLE_TESTING)
    include(FetchContent)

    enable_testing()

    # Fetch GoogleTest
    FetchContent_Declare(
        googletest
        EXCLUDE_FROM_ALL
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG
            e39786088138f2749d64e9e90e0f9902daa77c40 # release-1.15.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Create the library target and named header set for beman_optional26
add_library(beman_optional26 STATIC)
add_library(beman_optional26::beman_optional26 ALIAS beman_optional26)
target_sources(
    beman_optional26
    PUBLIC FILE_SET beman_optional26_includes TYPE HEADERS BASE_DIRS src include
    # FILES ... ?
)

set_target_properties(
    beman_optional26
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON
)

if(OPTIONAL26_ENABLE_TESTING)
    # Create the library target and named header set for testing beman_optional26
    # and mark the set private
    add_executable(beman_optional26_test)
    target_sources(
        beman_optional26_test
        PRIVATE
            FILE_SET beman_optional26_test_headers
            TYPE HEADERS
            BASE_DIRS src
        # FILES ... ?
    )
endif()

add_subdirectory(src/beman/optional26)
add_subdirectory(include/beman/optional26)

add_subdirectory(examples)

# Coverage
configure_file("cmake/gcovr.cfg.in" gcovr.cfg @ONLY)

add_custom_target(
    process_coverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running gcovr to process coverage results"
    COMMAND mkdir -p coverage
    COMMAND gcovr --config gcovr.cfg .
)

# install
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PACKAGE_NAME}-version.cmake
    VERSION ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PACKAGE_NAME}.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PACKAGE_NAME}.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_PACKAGE_NAME}-version.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

set(CPACK_GENERATOR TGZ)
include(CPack)
