#include<iostream>
#include<vector>
#include<functional>
#include<algorithm>

template<typename T, typename F>
std::vector<T> &operator|(std::vector<T> &v, const F &func) {
    std::for_each(v.begin(), v.end(), func);
    return v;
}

int main() {
    std::vector v{ 1, 2, 3 };
    std::function f{ [](const int &i) {std::cout << i << ' '; } };
    auto f2 = [](int &i) {i *= i; };
    v | f2 | f;
}