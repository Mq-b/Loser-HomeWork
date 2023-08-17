#include <iostream>
#include <vector>
#include <functional>

auto&& operator|(auto&& left, auto&& right)
{
    for (auto&& var : left)
        right(var);
    return left;
}

int main() {
    std::vector v{ 1, 2, 3 };
    std::function f{ [](const int& i) { std::cout << i << ' '; } };
    auto f2 = [](int& i) {i *= i; };
    v | f2 | f;
}