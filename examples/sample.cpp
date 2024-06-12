#include <Beman/Optional26/optional.hpp>
#include <iostream>

int main() {
    beman::optional::optional<int> empty_opt{};
    if (!empty_opt) {
        std::cout << "empty_opt is empty!\n";
    }

    beman::optional::optional<int> opt{26};
    if (opt) {
        std::cout << "opt = " << *opt << "\n";
    }

    return 0;
}
