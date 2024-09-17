// examples/optional_ref.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/optional26/optional.hpp>

#include <string>

struct Cat {
    int catalog_index{0};
};

namespace std17 {

// Prior to C++26, the code would look like this.
// Using raw pointers to represent optional references.
// Note: Using smart pointers would also be a choice, but it involves ownership semantics.

Cat* find_cat(std::string) { return nullptr; }

Cat* do_it(Cat& cat) { return &cat; }

Cat* api() {
    Cat* cat = find_cat("Fido");
    if (cat != nullptr) {
        return do_it(*cat);
    }
    return nullptr;
}

} // namespace std17

namespace std26 {
// After C++26 with P2988R5, the code would look like this.
// Using directly optional to represent optional references.

beman::optional26::optional<Cat&> find_cat(std::string) { return {}; }

beman::optional26::optional<Cat&> do_it(Cat& cat) { return {cat}; }

beman::optional26::optional<Cat&> api() {
    beman::optional26::optional<Cat&> cat = find_cat("Fido");
    return cat.and_then(do_it);
}

} // namespace std26

int main() {
    // Example from P2988R5: optional reference.
    [[maybe_unused]] Cat*                              old_cat = std17::api();
    [[maybe_unused]] beman::optional26::optional<Cat&> new_cat = std26::api();

    return 0;
}

// # build example:
// $ cmake --workflow --preset gcc-14
//
// # run example:
// $ .build/gcc-14/examples/RelWithDebInfo/optional_ref
