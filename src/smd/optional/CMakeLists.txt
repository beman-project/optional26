add_library(optional STATIC "")

target_sources(
  optional
  PRIVATE
  optional.cpp
)

include(GNUInstallDirs)

target_include_directories(optional PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}>  # <prefix>/include/scratch
  )

install(
  TARGETS optional
  EXPORT ${TARGETS_EXPORT_NAME}1
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LOWER_PROJECT_NAME)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
  )

target_link_libraries(optional)

## Tests
add_executable(optional_test "")

target_sources(
  optional_test
  PRIVATE
  optional.t.cpp
  optional_ref.t.cpp
  optional_monadic.t.cpp
)

target_link_libraries(optional_test optional)
target_link_libraries(optional_test gtest)
target_link_libraries(optional_test gtest_main)

include(GoogleTest)
gtest_discover_tests(optional_test)
