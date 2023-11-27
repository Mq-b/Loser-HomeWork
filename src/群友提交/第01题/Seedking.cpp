#include <vector>
#include <functional>
#include <iostream>

template <typename I, typename F>
std::vector<I>& operator|(std::vector<I> &input, const F& func) {
    for (auto& i : input) {
        func(i);
    }
    return input;
}

int main(){
    std::vector v{1, 2, 3};
    std::function f {[](const int& i) {std::cout << i << ' '; } };
    auto f2 = [](int& i) {i *= i; };
    v | f2 | f;
}
