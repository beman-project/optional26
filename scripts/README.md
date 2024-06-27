# Local Scripts

## Local Linters

Run all linters on codebase example:

```shell
$ scripts/lint.sh -h
Usage: scripts/lint-all.sh
  -h, --help: Display help.
  -a, --all: Lint all files.
  -c, --cpp: Lint C++ files.
  -s, --shell: Lint shell scripts.
  -m, --markdown: Lint Markdown files.
  -y, --yaml: Lint YAML files.
  -C, --cmake: Lint CMake files.

$ scripts/lint.sh --all
Checking linters...
 -> clang-format...
 -> shellcheck...
 -> cmake-format...
 -> markdownlint...
 -> yamlfmt...
Checking linters finished: all linters installed.

Linting C++ files...
Done.

Linting shell files...
Done.

Linting CMake files...
Done.

Linting Markdown files...
Done.

Linting YAML files...
Done.
```

## Local Run Tests

Run all tests on codebase example:

```shell
$ scripts/run-tests.sh 
Usage: scripts/run-tests.sh
  -h, --help: Display help.
  -v, --verbose: Display verbose output.
  -f, --fresh: Run tests on a fresh build directory.
  -p, --preset: Run tests on a specific toolchain. Default: all
                Available presets: gcc-14 gcc-13 clang-19 clang-18 clang-17

$ scripts/run-tests.sh --preset gcc-18
Testing with etc/gcc-18-toolchain.cmake...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Done.

$ scripts/run-tests.sh --preset all
Testing with etc/gcc-14-toolchain.cmake...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Testing with etc/gcc-13-toolchain.cmake...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Testing with etc/clang-19-toolchain.cmake...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Testing with etc/clang-18-toolchain.cmake...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Testing with etc/clang-17-toolchain.cmake...
Running: config
 -> config successful
Running: build
 -> build successful
Running: tests
 -> tests passed
Done.
```
