# Beman.Optional26 Examples

<!--
SPDX-License-Identifier: 2.0 license with LLVM exceptions
-->

List of usage examples for `Beman.Optional26`.

## Sample

Check [sample](sample.cpp) for basic `Beman.Optional26` library usage.

Build and run instructions:
```shell
# build
$ cmake --workflow --preset gcc-14

# run sample.cpp
$ .build/gcc-14/examples/RelWithDebInfo/sample
empty_opt is empty!
opt = 26

# run std_vs_beman.cpp
$ .build/gcc-14/examples/RelWithDebInfo/std_vs_beman
std_vs_beman: .has_value() matches?: yes
std_vs_beman: .value() matches?: yes
```

## Range Support (P3168R1)

Range support added in [*Give std::optional Range Support* (P3168R1)](https://wg21.link/P3168R1) examples:
* [./range_loop.cpp](./range_loop.cpp)
* [./pythagorean_triples.cpp](./pythagorean_triples.cpp)


Build and run instructions:

```shell


# run range_loop.cpp
$ .build/gcc-14/examples/RelWithDebInfo/range_loop
                                               # note: 1st log (empty_opt) is missing from console!
"for each loop" over C++26 optional: opt = 26  # 2nd log (non empty opt)

# run pythagorean_triples.cpp
$ .build/gcc-14/examples/RelWithDebInfo/pythagorean_triples
First 10 Pythagorean triples:
(3, 4, 5)
(6, 8, 10)
(5, 12, 13)
(9, 12, 15)
(8, 15, 17)
(12, 16, 20)
(7, 24, 25)
(15, 20, 25)
(10, 24, 26)
(20, 21, 29)
100th Pythagorean triple (x, y, z <= 200): (26, 168, 170)
```

## Optional Reference (P2988R5)

Reference support added in [*std::optional<T&>*(P2988R5)](https://wg21.link/P2988R5) examples:

* [./optional_ref.cpp](./optional_ref.cpp)

Build and run instructions:

```shell
# build
$ cmake --workflow --preset gcc-14

# run optional_ref.cpp
$ .build/gcc-14/examples/RelWithDebInfo/optional_ref
```
