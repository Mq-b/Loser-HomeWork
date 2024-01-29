#include<iostream>
#include<vector>
#include<functional>

template<typename T, typename Formatter>
T &operator|(T &arr, const Formatter &func) {
    std::for_each(std::begin(arr), std::end(arr), func);
    return arr;
}

int main() {
    std::vector v{ 1, 2, 3 };
    std::function f{ [](const int &i) {std::cout << i << ' '; } };
    auto f2 = [](int &i) {i *= i; };
    v | f2 | f;
}