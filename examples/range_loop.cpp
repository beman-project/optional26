// examples/range_loop.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/optional26/optional.hpp>
#include <iostream>

int main() {
    // Example from P3168R2: basic range loop over C++26 optional.

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

    return 0;
}

// # build example:
// $ cmake --workflow --preset gcc-14
//
// # run example:
// $ .build/gcc-14/examples/RelWithDebInfo/range_loop
//                                                # note: 1st log (empty_opt) is missing from console!
// "for each loop" over C++26 optional: opt = 26  # 2nd log (non empty opt)
