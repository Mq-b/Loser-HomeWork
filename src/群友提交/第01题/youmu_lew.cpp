#include <iostream>
#include <vector>
#include <functional>

template<typename T, typename Func>
std::vector<T>& operator|(std::vector<T>& v, Func func) {
    for (auto& i : v) {
        func(i);
    }
    return v;
}

int main(){
    std::vector v{1, 2, 3};
    std::function f {[](const int& i) {std::cout << i << ' '; } };
    auto f2 = [](int& i) {i *= i; };
    v | f2 | f;
}

// 预期结果： 1 4 9
