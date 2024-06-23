# Beman.Optional26 Examples

<!--
SPDX-License-Identifier: 2.0 license with LLVM exceptions
-->

List of usage examples for `Beman.Optional26`.

## License
Source is licensed with the Apache 2.0 license with LLVM exceptions

// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

Documentation and associated papers are licensed with the Creative Commons Attribution 4.0 International license.

// SPDX-License-Identifier: CC-BY-4.0

The intent is that the source and documentation are available for use by people implementing their own optional types as well as people using the optional presented here as-is.

The README itself is licesed with CC0 1.0 Universal. Copy the contents and incorporate in your own work as you see fit.

// SPDX-License-Identifier: CC0-1.0

## Sample

Check [sample](sample.cpp) for basic `Beman.Optional26` library usage.

Build and run instructions:
```shell
# build
$ cmake --workflow --preset gcc-14

# run sample
$ .build/gcc-14/examples/RelWithDebInfo/sample
empty_opt is empty!
opt = 26
```

## Range Support (P3168R1)

Range support added in [*Give std::optional Range Support* (P3168R1)](https://wg21.link/P3168R1) examples:
* [./range_loop.cpp](./range_loop.cpp)


Build and run instructions:

```shell
# build
$ cmake --workflow --preset gcc-14

# run range_loop
$ .build/gcc-14/examples/RelWithDebInfo/range_loop
                                               # note: 1st log (empty_opt) is missing from console!
"for each loop" over C++26 optional: opt = 26  # 2nd log (non empty opt)
```

## Optional Reference (P2988R5)

Reference support added in [*std::optional<T&>*(P2988R5)](https://wg21.link/P2988R5) examples:

* [./optional_ref.cpp](./optional_ref.cpp)
