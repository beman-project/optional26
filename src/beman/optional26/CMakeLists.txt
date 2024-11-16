# src/beman/optional26/CMakeLists.txt -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Ensure that optional and iterator get compiled at least once
target_sources(beman_optional26 PUBLIC optional.cpp detail/iterator.cpp)

# The library is empty -- exclude it
install(
    TARGETS beman_optional26
    EXPORT ${TARGETS_EXPORT_NAME}
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT beman_optional26_library
    FILE_SET beman_optional26_includes
)

install(
    TARGETS beman_optional26
    FILE_SET beman_optional26_headers
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT beman_optional26_development
)

# FIXME: install TARGETS target beman_optional26 is exported
#        but not all of its interface file sets are installed
# install(
#     EXPORT ${TARGETS_EXPORT_NAME}
#     FILE ${TARGETS_EXPORT_NAME}.cmake
#     DESTINATION "${INSTALL_CONFIGDIR}"
#     NAMESPACE beman_optional26::
# )

# Tests
if(OPTIONAL26_ENABLE_TESTING)
    add_subdirectory(tests)
endif()
