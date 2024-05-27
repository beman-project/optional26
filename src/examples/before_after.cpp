#include <beman/optional/optional.hpp>

#include <string>

using beman::optional::optional;

struct Cat {
    int catalog_index{0};
};

auto find_cat_old(std::string) -> Cat* { return nullptr; }

auto find_cat_new(std::string) -> optional<Cat&> { return optional<Cat&>{}; }

Cat* doit_old(Cat& cat) { return &cat; }
optional<Cat&> doit(Cat& cat) { return optional<Cat&>{cat}; }

Cat* before1() {
    Cat* cat = find_cat_old("Fido");
    if (cat != nullptr) {
        return doit_old(*cat);
    }
    return nullptr;
}

optional<Cat&> after1() {
    optional<Cat&> cat = find_cat_new("Fido");
    return cat.and_then(doit);
}

int main() {
    Cat*           cat_p = before1();
    optional<Cat&> cat_r = after1();
}
