# cmake-format: off
# examples/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

# List of all buildable examples.
set(EXAMPLES
    concept_checks
    optional_ref
    pythagorean_triples
    range_loop
    sample
    std_vs_beman
)

foreach(example ${EXAMPLES})
    # Add example executable.
    add_executable(${example} "")

    # Add example source file.
    target_sources(${example} PRIVATE ${example}.cpp)

    # Link example with the library.
    target_link_libraries(${example} beman_optional26::beman_optional26)

    # Install .
    install(
        TARGETS
            ${example}
            COMPONENT
            beman_optional26_examples
            DESTINATION
            ${CMAKE_INSTALL_BINDIR}
    )
endforeach()
