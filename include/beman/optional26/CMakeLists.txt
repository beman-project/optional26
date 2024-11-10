# include/beman/optional26/CMakeLists.txt -*-cmake-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

target_sources(
    beman_optional26
    PUBLIC
        FILE_SET beman_optional26_headers
        TYPE HEADERS
        FILES
            optional.hpp
            detail/iterator.hpp
            detail/stl_interfaces/config.hpp
            detail/stl_interfaces/fwd.hpp
            detail/stl_interfaces/iterator_interface.hpp
)

install(
    TARGETS beman_optional26
    FILE_SET beman_optional26_headers
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT beman_optional26_development
)
