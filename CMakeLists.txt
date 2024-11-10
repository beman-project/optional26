# CMakeLists.txt                                                    -*-CMake-*-
#
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.27)

project(beman_optional26 VERSION 0.0.0 LANGUAGES CXX)

# Includes
include(CTest)
include(FetchContent)

set(TARGETS_EXPORT_NAME ${CMAKE_PROJECT_NAME}Targets)

option(
    OPTIONAL26_ENABLE_TESTING
    "Enable building tests and test infrastructure"
    ${PROJECT_IS_TOP_LEVEL}
)

# Build the tests if enabled via the option OPTIONAL26_ENABLE_TESTING
if(OPTIONAL26_ENABLE_TESTING)
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

set(CMAKE_VERIFY_INTERFACE_HEADER_SETS ON)

# Create the library target and named header set for beman_optional26
add_library(beman_optional26 INTERFACE)
target_sources(
    beman_optional26
    PUBLIC FILE_SET beman_optional26_headers TYPE HEADERS BASE_DIRS src include
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
