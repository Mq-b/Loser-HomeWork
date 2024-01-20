//!gcc
#include <iostream>

// 可算是一种新思路？直接检查能否结构化绑定而非检查能否聚合初始化。
// 此代码可在 msvc 17 以及 clang 16 上编译，gcc 13 会报错。

template<class T> constexpr size_t size() {
    return 
        requires { [](const T& t) {auto& [a] = t; }; } ? 1 :
        requires { [](const T& t) {auto& [a, b] = t; }; } ? 2 :
        requires { [](const T& t) {auto& [a, b, c] = t; }; } ? 3 :
        requires { [](const T& t) {auto& [a, b, c, d] = t; }; } ? 4 :
        requires { [](const T& t) {auto& [a, b, c, d, e] = t; }; } ? 5 :
        0;
}

template<class T, class F>
void for_each_member(T&& t, F&& fn) {
    if constexpr (size<T>() == 1) {
        auto& [a] = t;
        fn(a);
    }
    else if constexpr (size<T>() == 2) {
        auto& [a,b] = t;
        fn(a), fn(b);
    }
    else if constexpr (size<T>() == 3) {
        auto& [a, b, c] = t;
        fn(a), fn(b), fn(c);
    }
    else if constexpr (size<T>() == 4) {
        auto& [a, b, c, d] = t;
        fn(a), fn(b), fn(c), fn(d);
    }
    else if constexpr (size<T>() == 5) {
        auto& [a, b, c, d, e] = t;
        fn(a), fn(b), fn(c), fn(d), fn(e);
    }
}

int main() {
    auto print = [](auto elem) {
        std::cout << elem << ' ';
    };
    struct X { std::string s = "Hello world."; }x;
    struct Y { int a{114}, b{514}, c{1919}, d{810}; }y;

    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    for_each_member(x, print); std::cout << '\n';
    for_each_member(y, print); std::cout << '\n';
}
