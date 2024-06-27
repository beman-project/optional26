# cmake-format: off
# src/Beman/Optional26/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

set(TARGET_LIBRARY "beman_optional26")

add_library("${TARGET_LIBRARY}" STATIC "")

target_sources("${TARGET_LIBRARY}" PRIVATE optional.cpp detail/iterator.cpp)

include(GNUInstallDirs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(
  "${TARGET_LIBRARY}"
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}> # <prefix>/include/scratch
)

install(
  TARGETS "${TARGET_LIBRARY}"
  EXPORT ${TARGETS_EXPORT_NAME}1
  DESTINATION ${CMAKE_INSTALL_LIBDIR})

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LOWER_PROJECT_NAME)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}
  FILES_MATCHING
  PATTERN "*.hpp")

target_link_libraries("${TARGET_LIBRARY}")

# Tests
add_executable(optional_test "")

target_sources(
  optional_test
  PRIVATE tests/optional.t.cpp tests/optional_ref.t.cpp
          tests/optional_monadic.t.cpp tests/optional_range_support.t.cpp
          tests/optional_ref_monadic.t.cpp tests/detail/iterator.t.cpp)

target_link_libraries(optional_test "${TARGET_LIBRARY}")
target_link_libraries(optional_test gtest)
target_link_libraries(optional_test gtest_main)

include(GoogleTest)
gtest_discover_tests(optional_test)
