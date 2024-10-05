// examples/sample.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/optional26/optional.hpp>
#include <iostream>

int main() {
    beman::optional26::optional<int> empty_opt{};
    if (!empty_opt) {
        std::cout << "empty_opt is empty!\n";
    }

    beman::optional26::optional<int> opt{26};
    if (opt) {
        std::cout << "opt = " << *opt << "\n";
    }

    return 0;
}

// # build example:
// $ cmake --workflow --preset gcc-14
//
// # run example:
// $ .build/gcc-14/examples/RelWithDebInfo/sample
// empty_opt is empty!
// opt = 26
