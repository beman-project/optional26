#include <Beman/Optional26/optional.hpp>
#include <iostream>

int main() {
    beman::optional::optional<int> empty_opt{};
    for ([[maybe_unused]] const auto& i : empty_opt) {
        // Should not see this in console.
        std::cout << "\"for each loop\" over C++26 optional: empty_opt\n";
    }

    beman::optional::optional<int> opt{26};
    for (const auto& i : opt) {
        // Should see this in console.
        std::cout << "\"for each loop\" over C++26 optional: opt = " << i << "\n";
    }
    return 0;
}
