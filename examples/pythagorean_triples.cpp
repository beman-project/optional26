// examples/pythagorean_triples.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/optional26/optional.hpp>

#include <algorithm>
#include <cassert>
#include <concepts>
#include <iostream>
#include <ranges>

int main() {
    // Example from P3168R2: generate an infinite sequence of Pythagorean triples.
    // (x, y, z) is a Pythagorean triple if 1 <= x <= y <= z and x^2 + y^2 = z^2.
    constexpr auto yield_if = []<class T>(bool b, T x) -> beman::optional26::optional<T> {
        return b ? beman::optional26::optional<T>{std::move(x)} : beman::optional26::nullopt;
    };
    constexpr auto and_then = []<class T>(T&& r, auto fun) {
        return decltype(r)(r) | std::views::transform(std::move(fun)) | std::views::join;
    };
    auto triples = and_then(std::views::iota(1), [=](int z) {
        return and_then(std::views::iota(1, z + 1), [=](int x) {
            return and_then(std::views::iota(x, z + 1),
                            [=](int y) { return yield_if(x * x + y * y == z * z, std::make_tuple(x, y, z)); });
        });
    });

    {
        // Generate first 10 Pythagorean triples.
        // https://mathworld.wolfram.com/PythagoreanTriple.html
        [[maybe_unused]] auto&& r = triples | std::views::take(10);
        assert(std::ranges::equal(r,
                                  std::vector{
                                      std::tuple{3, 4, 5},
                                      std::tuple{6, 8, 10},
                                      std::tuple{5, 12, 13},
                                      std::tuple{9, 12, 15},
                                      std::tuple{8, 15, 17},
                                      std::tuple{12, 16, 20},
                                      std::tuple{7, 24, 25},
                                      std::tuple{15, 20, 25},
                                      std::tuple{10, 24, 26},
                                      std::tuple{20, 21, 29},
                                  }));

        std::cout << "First 10 Pythagorean triples:\n";
        for (const auto& [x, y, z] : r) {
            std::cout << "(" << x << ", " << y << ", " << z << ")\n";
        }
    }

    {
        // Generate the nth Pythagorean triple (x, y, z) where:
        // 1 <= x <= y <= z <= max_value and x^2 + y^2 = z^2

        // Generate the nth Pythagorean triple with brute force.
        [[maybe_unused]] auto bruteforce_generate_nth = [](int n, int max_value) -> std::tuple<int, int, int> {
            int count = 0; // Count the number of Pythagorean triples.
            for (int z = 1; z <= max_value; ++z) {
                for (int x = 1; x <= max_value; ++x) {
                    for (int y = 1; y <= max_value; ++y) {
                        if (!(x <= y && y <= z) || x * x + y * y != z * z) {
                            continue;
                        }

                        if (++count == n) {
                            return std::make_tuple(x, y, z);
                        }
                    }
                }
            }

            return {}; // Should not reach here.
        };
        [[maybe_unused]] constexpr const std::tuple k100th_triple = {
            26, 168, 170}; // The 100th Pythagorean triple with x, y, z <= 200.
        assert(bruteforce_generate_nth(100, 200) == k100th_triple);

        // Generate the 100th Pythagorean triple with ranges.
        [[maybe_unused]] auto&& r = triples | std::views::drop(99) | std::views::take(1);
        assert(std::ranges::equal(r, std::vector{k100th_triple}));

        const auto& [x100, y100, z100] = *std::ranges::begin(r);
        std::cout << "100th Pythagorean triple (x, y, z <= 200): (" << x100 << ", " << y100 << ", " << z100 << ")\n";
    }

    return 0;
}

// # build example:
// $ cmake --workflow --preset gcc-14
//
// # run example:
// $ .build/gcc-14/examples/RelWithDebInfo/pythagorean_triples
// First 10 Pythagorean triples:
// (3, 4, 5)
// (6, 8, 10)
// (5, 12, 13)
// (9, 12, 15)
// (8, 15, 17)
// (12, 16, 20)
// (7, 24, 25)
// (15, 20, 25)
// (10, 24, 26)
// (20, 21, 29)
// 100th Pythagorean triple (x, y, z <= 200): (26, 168, 170)
