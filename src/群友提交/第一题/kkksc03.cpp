#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>
#include <ranges>

template <typename Range, typename Function>
auto operator|(Range&& r, Function&& f) {
    std::ranges::for_each(r, f);
    return r;
}
int main() {
    std::vector v{1, 2, 3};
    std::function f{[](const int& i) { std::cout << i << ' '; }};
    auto f2 = [](int& i) { i *= i; };
    v | f2 | f;
} 
