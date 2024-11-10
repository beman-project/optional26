# src/beman/optional26/CMakeLists.txt -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# The library is empty -- exclude it
install(
    TARGETS beman_optional26
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT beman_optional26_library
        EXCLUDE_FROM_ALL
)

install(
    TARGETS beman_optional26
    FILE_SET beman_optional26_headers
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT beman_optional26_development
)

# Tests
if(OPTIONAL26_ENABLE_TESTING)
    add_subdirectory(tests)
endif()
