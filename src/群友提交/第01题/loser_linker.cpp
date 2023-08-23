#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <concepts>

template<typename T, std::invocable<T&> F>
std::vector<T>& operator|(std::vector<T>& vs, F const & f) {
    std::ranges::for_each(vs, f);

    return vs;
}

int main() {
    std::vector v{1, 2, 3};

    std::function f { [](int const & i) { std::cout << i << ' '; } };
    auto f2 = [](int& i) { i *= i; };
    
    v | f2 | f;
}