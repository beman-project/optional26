# OPTIONAL<T&>

![](https://github.com/steve-downey/view_maybe/actions/workflows/ci.yml/badge.svg)

A proof of concept for the std::optional<T&> proposal.

Source is licensed with the Apache 2.0 license with LLVM exceptions

// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

Documentation and associated papers are licensed with the Creative Commons Attribution 4.0 International license.

// SPDX-License-Identifier: CC-BY-4.0

The intent is that the source and documentation are available for use by people implementing their own optional types as well as people using the optional presented here as-is.

The README itself is licesed with CC0 1.0 Universal. Copy the contents and incorporate in your own work as you see fit.

// SPDX-License-Identifier: CC0-1.0

## Building Optional

### Dependencies

This project is mainly tested on Ubuntu 22.04, but it should be as portable as CMake is.

This project has zero C or C++ depenendencies.

It does require two tools as build-time dependencies:

-  `cmake`
- `ninja`, `make`, or another CMake-supported build system
  - CMake defaults to "Unix Makefiles" on POSIX systems

Compiling the paper requires a working LaTeX installation. See instructions for configuring your system at [C++ Standard Draft Sources](https://github.com/cplusplus/draft/blob/main/README.rst)

### Instructions

#### Basic Build

This project strives to be as normal and simple a CMake project as possible. This build workflow in particular will work, producing a static `example` library, ready to package:

```shell
cmake --workflow --preset gcc-14
```
This should build and run the tests with GCC 14 with the address and undefined behavior sanitizers enabled.

#### More complex cases
The CMake preset system suffers from combinitorial explosion. There is a makefile in the root of the repository to aid in running more configurations.

```shell
make -k TOOLCHAIN=clang-18  CONFIG=Tsan  VERBOSE=1
```

The makefile will use your system compiler, `c++`, if no toolchain name is provided, otherwise it will use the toolchain in the etc/ directory to perform the build. The Ninja multi config generator is used, with configurations for RelWithDebugInfo, Debug, Tsan, and Asan configured by default.

#### Building P2988

The papers/ subdirectory has the LaTeX souces for P2988 and the supporting macro definitions. To build, run
```shell
make papers
```

A working recent Python 3 is required to format the sources for the paper. A virtual env will be created in the papers subdirectory which the `minted` LaTeX package will use.

`
