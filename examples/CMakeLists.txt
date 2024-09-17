# cmake-format: off
# examples/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

include(GNUInstallDirs)

# List of all buildable examples.
set(EXAMPLES concept_checks optional_ref pythagorean_triples range_loop sample
             std_vs_beman)

foreach(example ${EXAMPLES})
  # Add example executable.
  add_executable(beman.optional26.examples.${example} "")

  # Add example source file.
  target_sources(beman.optional26.examples.${example} PRIVATE ${example}.cpp)

  # Link example with the library.
  target_link_libraries(beman.optional26.examples.${example} beman.optional26)

  # Install .
  install(
    TARGETS beman.optional26.examples.${example}
    EXPORT ${TARGETS_EXPORT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR})
endforeach()
