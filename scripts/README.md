# Local Scripts

## Local Run Tests

<details open>
<summary> scripts/run-tests.sh dependencies </summary>

`scripts/run-tests.sh` requires linters to be installed:
* `C++`: [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
* `Shell`: [shellcheck](https://www.shellcheck.net/)
* `Markdown`: [markdownlint](https://github.com/DavidAnson/markdownlint)
* `YAML`: [yamllint](https://yamllint.readthedocs.io/en/stable/)
* `Cmake`: [cmake-format](https://cmake-format.readthedocs.io/en/latest/)

```shell
# Linter install example on Ubuntu 24.04:

# Install clang-format with https://apt.llvm.org/llvm.sh or apt
$ sudo apt-get install clang-format-18

# Install shellcheck with apt-get
$ sudo apt-get install shellcheck

# Install markdownlint-cli with npm
$ sudo npm install -g markdownlint-cli

# Install yamllint with apt
$ sudo apt-get install yamllint

# Install cmake-format with apt
$ sudo apt-get install cmake-format
```
</details>


<details open>
<summary> scripts/run-tests.sh: print usage </summary>

```shell
# Print usage.
$ scripts/run-tests.sh 
Usage: scripts/run-tests.sh
  -h, --help: Display help.
  -v, --verbose: Display verbose output.
  -f, --fresh: Run tests on a fresh build directory. Default: incremental build
  -p, --preset: Run tests on a specific toolchain. Default: all
                Available presets: gcc-14 gcc-13 clang-19 clang-18 clang-17
  -c, --config: Run tests on a specific configuration. Default: RelWithDebInfo
                Available configurations: Debug Release RelWithDebInfo Asan
  -s, --std: Run tests with a specific C++ standard. Default: 23
                Available standards: 20 23 26
```
</details>

<details>
<summary>Run all tests (successful job): preset gcc-14, standard C++20. </summary>

```shell
# Successful job, non-verbose mode.
$ scripts/run-tests.sh --preset gcc-14 --std 20
Testing with etc/gcc-14-toolchain.cmake...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Done.

scripts/run-tests.sh all jobs are done.

# Successful job, verbose mode.
$ scripts/run-tests.sh --preset gcc-14 --std 20 --verbose
Testing with etc/gcc-14-toolchain.cmake...
Running: config
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /path/to/repo/Beman.Optional26/.build
 -> config successful
Running: build
[5/26] Building CXX object src/Beman/Optional26/CMakeFiles/optional_test.dir/RelWithDebInfo/tests/optional_ref.t.cpp.o
...
/path/to/repo/Beman.Optional26/src/Beman/Optional26/tests/optional.t.cpp:42:38: warning: unused variable 'i2' [-Wunused-variable]
   42 |     beman::optional26::optional<int> i2{beman::optional26::nullopt};
...
[26/26] Linking CXX executable ...
 -> build successful
Running: tests
Test project /path/to/repo/Beman.Optional26/.build
      Start  1: OptionalTest.TestGTest
 1/76 Test  #1: OptionalTest.TestGTest ...........................   Passed    0.00 sec
...
      Start 76: IteratorTest.ContainerType
76/76 Test #76: IteratorTest.ContainerType .......................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 76

Total Test time (real) =   0.07 sec
 -> tests passed

scripts/run-tests.sh all jobs are done.
```
</details>

<details>
<summary>Run all tests (failed job): preset clang-19, standard C++23.</summary>

```shell
# Failed job, non-verbose mode.
$ scripts/run-tests.sh --preset clang-19 --std 23
Testing with etc/clang-19-toolchain.cmake...
Running: config
 -> config successful
Running: build
 -> build failed

# Failed job, verbose mode.
$ scripts/run-tests.sh --preset clang-19 --std 23 --verbose
Testing with etc/clang-19-toolchain.cmake...
Running: config
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/dariusn/git/Beman/Beman.Optional26/.build
 -> config successful
Running: build
[1/9] Building CXX object src/Beman/Optional26/CMakeFiles/beman_optional26.dir/RelWithDebInfo/optional.cpp.o
FAILED: src/Beman/Optional26/CMakeFiles/beman_optional26.dir/RelWithDebInfo/optional.cpp.o 
/usr/bin/clang++-19 -DCMAKE_INTDIR=\"RelWithDebInfo\" -I/home/dariusn/git/Beman/Beman.Optional26/src/Beman/Optional26 -I/home/dariusn/git/Beman/Beman.Optional26/src/Beman/Optional26/../../../include -std=c++23 -Wall -Wextra -stdlib=libc++ -O3 -g -DNDEBUG -std=gnu++23 -MD -MT src/Beman/Optional26/CMakeFiles/beman_optional26.dir/RelWithDebInfo/optional.cpp.o -MF src/Beman/Optional26/CMakeFiles/beman_optional26.dir/RelWithDebInfo/optional.cpp.o.d -o src/Beman/Optional26/CMakeFiles/beman_optional26.dir/RelWithDebInfo/optional.cpp.o -c /home/dariusn/git/Beman/Beman.Optional26/src/Beman/Optional26/optional.cpp
/home/dariusn/git/Beman/Beman.Optional26/src/Beman/Optional26/optional.cpp:6:1: error: expected unqualified-id
    6 | this code should not compile
      | ^
1 error generated.
ninja: build stopped: cannot make progress due to previous errors.
 -> build failed
```
</details>

<details>
<summary>Run all tests: all toolchains (with fresh build directory for each toolchain), standard C++26.</summary>

```shell
# Run all tests on all toolchains with C++26.
# Inspect passed and tests jobs (toolchains).
$ $ scripts/run-tests.sh --preset all --std 26  
Testing with etc/gcc-14-toolchain.cmake...
[WARNING] Removing existing build directory: .build ...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Done testing with etc/gcc-14-toolchain.cmake.

Testing with etc/gcc-13-toolchain.cmake...
[WARNING] Removing existing build directory: .build ...
Running: config
 -> config failed
Done testing with etc/gcc-13-toolchain.cmake.

Testing with etc/clang-19-toolchain.cmake...
[WARNING] Removing existing build directory: .build ...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Done testing with etc/clang-19-toolchain.cmake.

Testing with etc/clang-18-toolchain.cmake...
[WARNING] Removing existing build directory: .build ...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Done testing with etc/clang-18-toolchain.cmake.

Testing with etc/clang-17-toolchain.cmake...
[WARNING] Removing existing build directory: .build ...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Done testing with etc/clang-17-toolchain.cmake.

scripts/run-tests.sh all jobs are done.
```
</details>

 ## Local Linters

<details open>
<summary> scripts/lint.sh: print usage </summary>

```shell
# Print usage.
$ scripts/lint.sh 
Usage: scripts/lint.sh
  -h, --help: Display help.
  -f, --fix: Fix all linting issues. Default: false.
  -v, --verbose: Enable verbose output.
  -a, --all: Lint all files.
  -c, --cpp: Lint C++ files.
  -s, --shell: Lint shell scripts.
  -m, --markdown: Lint Markdown files.
  -y, --yaml: Lint YAML files.
  -C, --cmake: Lint CMake files
```
</details>

<details>
<summary> Run linter for all C++ files </summary>

```shell
# Run linter (report mode): no errors found
$ scripts/lint.sh --cpp
Linting C++ files...
Formatting [1/20] include/Beman/Optional26/detail/stl_interfaces/fwd.hpp
Formatting [2/20] include/Beman/Optional26/detail/stl_interfaces/config.hpp
Formatting [3/20] include/Beman/Optional26/detail/stl_interfaces/iterator_interface.hpp
Formatting [4/20] include/Beman/Optional26/detail/iterator.hpp
Formatting [5/20] include/Beman/Optional26/optional.hpp
Formatting [6/20] examples/range_loop.cpp
Formatting [7/20] examples/sample.cpp
Formatting [8/20] examples/std_vs_beman.cpp
Formatting [9/20] examples/concept_checks.cpp
Formatting [10/20] examples/pythagorean_triples.cpp
Formatting [11/20] examples/optional_ref.cpp
Formatting [12/20] src/Beman/Optional26/detail/iterator.cpp
Formatting [13/20] src/Beman/Optional26/tests/optional_ref_monadic.t.cpp
Formatting [14/20] src/Beman/Optional26/tests/detail/iterator.t.cpp
Formatting [15/20] src/Beman/Optional26/tests/test_types.hpp
Formatting [16/20] src/Beman/Optional26/tests/optional_monadic.t.cpp
Formatting [17/20] src/Beman/Optional26/tests/optional.t.cpp
Formatting [18/20] src/Beman/Optional26/tests/optional_range_support.t.cpp
Formatting [19/20] src/Beman/Optional26/tests/optional_ref.t.cpp
Formatting [20/20] src/Beman/Optional26/optional.cpp
Done.

# Run linter (report mode): errors found.
$ scripts/lint.sh --cpp
scripts/lint.sh --cpp
Linting C++ files...
Formatting [1/20] include/Beman/Optional26/detail/stl_interfaces/fwd.hpp
Formatting [2/20] include/Beman/Optional26/detail/stl_interfaces/config.hpp
Formatting [3/20] include/Beman/Optional26/detail/stl_interfaces/iterator_interface.hpp
Formatting [4/20] include/Beman/Optional26/detail/iterator.hpp
Formatting [5/20] include/Beman/Optional26/optional.hpp
Formatting [6/20] examples/range_loop.cpp
Formatting [7/20] examples/sample.cpp
Formatting [8/20] examples/std_vs_beman.cpp
Formatting [9/20] examples/concept_checks.cpp
examples/concept_checks.cpp:17:22: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor()                                  
                     ^
examples/concept_checks.cpp:18:6: error: code should be clang-formatted [-Wclang-format-violations]
    = 
     ^
examples/concept_checks.cpp:20:42: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor(const no_default_ctor&)            = default;
                                         ^
examples/concept_checks.cpp:21:36: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor(no_default_ctor&&)                 = default;
                                   ^
examples/concept_checks.cpp:22:20: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor& operator=(const no_default_ctor&) = default;
                   ^
examples/concept_checks.cpp:22:21: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor& operator=(const no_default_ctor&) = default;
                    ^
examples/concept_checks.cpp:22:53: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor& operator=(const no_default_ctor&) = default;
                                                    ^
examples/concept_checks.cpp:23:20: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor& operator=(no_default_ctor&&)      = default;
                   ^
examples/concept_checks.cpp:23:21: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor& operator=(no_default_ctor&&)      = default;
                    ^
examples/concept_checks.cpp:23:47: error: code should be clang-formatted [-Wclang-format-violations]
    no_default_ctor& operator=(no_default_ctor&&)      = default;
                                              ^
examples/concept_checks.cpp:43:44: error: code should be clang-formatted [-Wclang-format-violations]
const auto test_concepts_disabled = [](auto&& opt) {
                                           ^
examples/concept_checks.cpp:43:46: error: code should be clang-formatted [-Wclang-format-violations]
const auto test_concepts_disabled = [](auto&& opt) {
                                             ^
examples/concept_checks.cpp:60:43: error: code should be clang-formatted [-Wclang-format-violations]
const auto test_concepts_enabled = [](auto&& opt) {
                                          ^
examples/concept_checks.cpp:60:45: error: code should be clang-formatted [-Wclang-format-violations]
const auto test_concepts_enabled = [](auto&& opt) {
                                            ^
examples/concept_checks.cpp:82:49: error: code should be clang-formatted [-Wclang-format-violations]
        test_concepts_disabled(std::optional<int*>{});
                                                ^
examples/concept_checks.cpp:92:62: error: code should be clang-formatted [-Wclang-format-violations]
        test_concepts_enabled(beman::optional26::optional<int*>{});
                                                             ^
Formatting [10/20] examples/pythagorean_triples.cpp
Formatting [11/20] examples/optional_ref.cpp
Formatting [12/20] src/Beman/Optional26/detail/iterator.cpp
Formatting [13/20] src/Beman/Optional26/tests/optional_ref_monadic.t.cpp
Formatting [14/20] src/Beman/Optional26/tests/detail/iterator.t.cpp
Formatting [15/20] src/Beman/Optional26/tests/test_types.hpp
Formatting [16/20] src/Beman/Optional26/tests/optional_monadic.t.cpp
Formatting [17/20] src/Beman/Optional26/tests/optional.t.cpp
Formatting [18/20] src/Beman/Optional26/tests/optional_range_support.t.cpp
Formatting [19/20] src/Beman/Optional26/tests/optional_ref.t.cpp
Formatting [20/20] src/Beman/Optional26/optional.cpp
src/Beman/Optional26/optional.cpp:6:6: error: code should be clang-formatted [-Wclang-format-violations]
int x      = 0

# Run linter (fix mode).
scripts/lint.sh --cpp --fix
Linting C++ files...
Formatting [1/20] include/Beman/Optional26/detail/stl_interfaces/fwd.hpp
Formatting [2/20] include/Beman/Optional26/detail/stl_interfaces/config.hpp
Formatting [3/20] include/Beman/Optional26/detail/stl_interfaces/iterator_interface.hpp
Formatting [4/20] include/Beman/Optional26/detail/iterator.hpp
Formatting [5/20] include/Beman/Optional26/optional.hpp
Formatting [6/20] examples/range_loop.cpp
Formatting [7/20] examples/sample.cpp
Formatting [8/20] examples/std_vs_beman.cpp
Formatting [9/20] examples/concept_checks.cpp
Formatting [10/20] examples/pythagorean_triples.cpp
Formatting [11/20] examples/optional_ref.cpp
Formatting [12/20] src/Beman/Optional26/detail/iterator.cpp
Formatting [13/20] src/Beman/Optional26/tests/optional_ref_monadic.t.cpp
Formatting [14/20] src/Beman/Optional26/tests/detail/iterator.t.cpp
Formatting [15/20] src/Beman/Optional26/tests/test_types.hpp
Formatting [16/20] src/Beman/Optional26/tests/optional_monadic.t.cpp
Formatting [17/20] src/Beman/Optional26/tests/optional.t.cpp
Formatting [18/20] src/Beman/Optional26/tests/optional_range_support.t.cpp
Formatting [19/20] src/Beman/Optional26/tests/optional_ref.t.cpp
Formatting [20/20] src/Beman/Optional26/optional.cpp
Done.
```
</details>

<details>
<summary> Run all linters on codebase </summary>

```shell
# Run all linters on codebase
$ scripts/lint.sh --fix --all
```
</details>

