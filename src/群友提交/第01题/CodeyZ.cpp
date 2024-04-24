#include<iostream>
#include<vector>
#include <functional>

template<typename V,typename T>
auto operator|(V&& v, T&& f) {
    for (auto &i : v) {
        f(i);
    }
    return v;
}


int main() {

    std::vector v{ 1, 2, 3 };
    std::function f{ [](const int& i) {std::cout << i << ' '; } };
    auto f2 = [](int& i) {i *= i; };
    v | f2 | f;
    return 0;
}