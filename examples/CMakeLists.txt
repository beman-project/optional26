# examples/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(BEMAN_OPTIONAL26_LIBRARY "beman_optional26")

include(GNUInstallDirs)

# List of all buildable examples.
set(EXAMPLES
  optional_ref
  pythagorean_triples
  range_loop
  sample
  std_vs_beman
)

foreach(EXAMPLE ${EXAMPLES})
  # Add example executable.
  add_executable(${EXAMPLE} "")

  # Add example source file.
  target_sources(
    ${EXAMPLE}
    PRIVATE
    ${EXAMPLE}.cpp
  )

  # Link example with the library.
  target_link_libraries(${EXAMPLE} "${BEMAN_OPTIONAL26_LIBRARY}")

  # Install .
  install(
    TARGETS ${EXAMPLE}
    EXPORT ${TARGETS_EXPORT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endforeach()
