# Beman.Optional26: C++26 Extensions for std::optional

<!--
SPDX-License-Identifier: 2.0 license with LLVM exceptions
-->

This repository implements `std::optional` extensions targeting C++26. The `Beman.Optional26` library aims to evaluate the stability, the usability, and the performance of these proposed changes before they are officially adopted by WG21 into the C++ Working Draft. Additionally, it allows developers to use these new features before they are implemented in major standard library compilers.

**Implements**:
* [Give *std::optional* Range Support (P3168R1)](https://wg21.link/P3168R1)
* [`std::optional<T&>` (P2988R5)](https://wg21.link/P2988R5)

## License

Source, documentation and associated papers are licensed with the Apache 2.0 license with LLVM exceptions.

// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception


## Examples

Full runable examples can be found in `examples/` - please check [./examples/README.md](./examples/README.md).

### range_loop

The next code snippet shows optional range support added in [Give *std::optional* Range Support (P3168R1)](https://wg21.link/P3168R1):

```cpp
#include <Beman/Optional26/optional.hpp>
...

// Example from P3168R1: basic range loop over C++26 optional.

beman::optional26::optional<int> empty_opt{};
for ([[maybe_unused]] const auto& i : empty_opt) {
    // Should not see this in console.
    std::cout << "\"for each loop\" over C++26 optional: empty_opt\n";
}

beman::optional26::optional<int> opt{26};
for (const auto& i : opt) {
    // Should see this in console.
    std::cout << "\"for each loop\" over C++26 optional: opt = " << i << "\n";
}
```

Full code can be found in [./examples/range_loop.cpp](./examples/range_loop.cpp). Build and run instructions in [./examples/README.md](./examples/README.md).

### optional_ref

The next code snippet shows optional reference support added in [`std::optional<T&>` (P2988R5)](https://wg21.link/P2988R5):

```cpp
#include <Beman/Optional26/optional.hpp>
...

{
    // Empty optional example.
    std::optional<int>             std_empty_opt;
    beman::optional26::optional<int> beman_empty_opt;
    assert(!std_empty_opt.has_value() &&
            !beman_empty_opt.has_value()); // or assert(!std_empty_opt && !beman_empty_opt);
    std::cout << "std_vs_beman: .has_value() matches?: "
              << (std_empty_opt.has_value() == beman_empty_opt.has_value() ? "yes" : "no") << "\n";
}

{
    // Optional with value example.
    std::optional<int>             std_opt   = 26;
    beman::optional26::optional<int> beman_opt = 26;
    assert(std_opt.has_value() && beman_opt.has_value()); // or assert(std_opt && beman_opt);
    assert(std_opt.value() == beman_opt.value());         // or assert(*std_opt == *beman_opt);
    std::cout << "std_vs_beman: .value() matches?: " << (std_opt.value() == beman_opt.value() ? "yes" : "no")
              << "\n";
}
```

Full code can be found in [./examples/optional_ref.cpp](./examples/optional_ref.cpp). Build and run instructions in [./examples/README.md](./examples/README.md).

## How to Build

### Compiler Support

This is a modern C++ project which can be compiled with the latest C++ standards (**C++20 or later**).

Default build: `C++23`. Please check `etc/${compiler}-flags.cmake`.

### Dependencies

This project is mainly tested on `Ubuntu 22.04` and `Ubuntu 24.04`, but it should be as portable as CMake is. This project has no C or C++ dependencies.

Build-time dependencies:

- `cmake`
- `ninja`, `make`, or another CMake-supported build system
  - CMake defaults to "Unix Makefiles" on POSIX systems

Example of installation on `Ubuntu 24.04`:
```shell
# Install tools:
apt-get install -y cmake make ninja-build

# Example of toolchains:
apt-get install                           \
  g++-14 gcc-14 gcc-13 g++-14             \
  clang-18 clang++-18 clang-17 clang++-17
```

### Instructions

Full set of supported toolchains can be found in [.github/workflows/ci.yml](#.github/workflows/ci.yml).

#### Basic Build

This project strives to be as normal and simple a CMake project as possible. This build workflow in particular will work, producing a static `beman_optional26` library, ready to package:

```shell
# List available preset configurations:
$ cmake --workflow --list-presets
Available workflow presets:

  "system"
  "gcc-14"
  "gcc-13"
  "clang-18"
  "clang-17"

# Run examples:
$ cmake --workflow --preset gcc-14
cmake --workflow --preset gcc-14
Executing workflow step 1 of 3: configure preset "gcc-14"
...
-- Build files have been written to: /path/to/repo/.build/gcc-14

Executing workflow step 2 of 3: build preset "gcc-14"

ninja: no work to do.

Executing workflow step 3 of 3: test preset "gcc-14"

Test project /path/to/repo/.build/gcc-14
        Start   1: OptionalTest.TestGTest
  1/... Test   #1: OptionalTest.TestGTest ...........................   Passed    0.00 sec
...
        Start   x: RangeSupportTest.RangeConcepts
.../... Test   #x: RangeSupportTest.RangeConcepts ...................   Passed    0.00 sec
        Start x+1: RangeSupportTest.IteratorConcepts
.../... Test #x+1: RangeSupportTest.IteratorConcepts ................   Passed    0.00 sec
...

100% tests passed, 0 tests failed out of ...

Total Test time (real) =   0.09 sec
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
* [give-std-optional-range-support](https://github.com/neatudarius/give-std-optional-range-support/) for `Give *std::optional* Range Support (P3168)`
    * issue: [#1831](https://github.com/cplusplus/papers/issues/1831)
  * LEWG:
    * Reviewed in Tokyo 2024.
    * Forwarded by LEWG April electronic poll to LWG.
  * LWG:
    * To be reviewed in Saint Louis 2024.
* [./papers/P2988/README.md](./papers/P2988/README.md) for `std::optional<T&> (P2988)`.
    * issue: [#1661](https://github.com/cplusplus/papers/issues/1661)
    * LEWG:
      * Reviewed in Tokyo 2024.
