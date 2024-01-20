//!all
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

auto operator | (auto&& T t, auto&& F f) {
    for(auto& item : T) {
        f(item);
    }
    return t;
}


int main() {
    std::vector v{1, 2, 3};
    std::function f{[](const int& i) { std::cout << i << ' '; }};
    auto f2 = [](int& i) { i *= i; };
    v | f2 | f;
}
