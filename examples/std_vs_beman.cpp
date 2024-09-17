// examples/std_vs_beman.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <optional>
#include <beman/optional26/optional.hpp>

#include <cassert>
#include <iostream>

int main() {
    {
        // Empty optional example.
        std::optional<int>               std_empty_opt;
        beman::optional26::optional<int> beman_empty_opt;
        assert(!std_empty_opt.has_value() &&
               !beman_empty_opt.has_value()); // or assert(!std_empty_opt && !beman_empty_opt);
        std::cout << "std_vs_beman: .has_value() matches?: "
                  << (std_empty_opt.has_value() == beman_empty_opt.has_value() ? "yes" : "no") << "\n";
    }

    {
        // Optional with value example.
        std::optional<int>               std_opt   = 26;
        beman::optional26::optional<int> beman_opt = 26;
        assert(std_opt.has_value() && beman_opt.has_value()); // or assert(std_opt && beman_opt);
        assert(std_opt.value() == beman_opt.value());         // or assert(*std_opt == *beman_opt);
        std::cout << "std_vs_beman: .value() matches?: " << (std_opt.value() == beman_opt.value() ? "yes" : "no")
                  << "\n";
    }

    // Note: std_empty_opt == beman_empty_opt and std_opt == beman_opt won't compile
    // (no implementation of operator== for std::optional and beman::optional26::optional).

    return 0;
}

// # build example:
// $ cmake --workflow --preset gcc-14
//
// # run example:
// $ .build/gcc-14/examples/RelWithDebInfo/std_vs_beman
// std_vs_beman: .has_value() matches?: yes
// std_vs_beman: .value() matches?: yes
