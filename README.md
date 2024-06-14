# Beman.Optional26: C++26 Extensions for std::optional

This implementation incorporates the C++26 extensions added for `std::optional`. The `Beman.Optional26` library aims to evaluate the stability, the usability, and the performance of these proposed changes before they are officially adopted by WG21 into the C++ Working Draft. Additionally, it allows developers to use these new features before they are implemented in major standard library compilers.

**Implements**:
* [`std::optional<T&>` (P2988R5)](https://wg21.link/P2988R5)
* [Give *std::optional* Range Support (P3168R1)](https://wg21.link/P3168R1).


Source is licensed with the Apache 2.0 license with LLVM exceptions

// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

Documentation and associated papers are licensed with the Creative Commons Attribution 4.0 International license.

// SPDX-License-Identifier: CC-BY-4.0

The intent is that the source and documentation are available for use by people implementing their own optional types as well as people using the optional presented here as-is.

The README itself is licesed with CC0 1.0 Universal. Copy the contents and incorporate in your own work as you see fit.

// SPDX-License-Identifier: CC0-1.0

## How to Build

### Dependencies

This project is mainly tested on `Ubuntu 22.04` and `Ubuntu 24.04`, but it should be as portable as CMake is. This project has zero C or C++ dependencies.

It does require few tools as build-time dependencies:

- `cmake`
- `ninja`, `make`, or another CMake-supported build system
  - CMake defaults to "Unix Makefiles" on POSIX systems

Example of installation on `Ubuntu 24.04`:
```shell
# install tools
apt-get install -y cmake make ninja-build

# example of toolchains
apt-get install g++-14 gcc-14 clang-18 clang++-18
```

### Instructions

Full set of supported toolchains can be found in [.github/workflows/ci.yml](#.github/workflows/ci.yml).

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

The makefile will use your system compiler, `c++`, if no toolchain name is provided, otherwise it will use the toolchain in the etc/ directory to perform the build. The Ninja multi config generator is used, with configurations for `RelWithDebugInfo`, `Debug`, `Tsan`, and `Asan` configured by default.

## Papers

Latest revision(s) of the papers can be built / found at:
* [./papers/P2988/README.md](./papers/P2988/README.md) for `std::optional<T&> (P2988)`.
* [give-std-optional-range-support](https://github.com/neatudarius/give-std-optional-range-support/) for `Give *std::optional* Range Support (P3168)`

