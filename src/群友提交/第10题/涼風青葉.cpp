#include<iostream>

struct Any {
    template<typename T>
    operator T();
};

template<typename T, typename ...Args>
constexpr std::size_t size(Args ...args) {
    if constexpr (requires {T{ args... };})
        return size<T>(args..., Any{});
    else
        return sizeof...(args) - 1;
}

template<typename T, typename Func>
void for_each_member(const T &arg, const Func &func) {
    if constexpr (size<T>() == 1) {
        const auto &[m1] = arg;
        func(m1);
    } else if constexpr (size<T>() == 2) {
        const auto &[m1, m2] = arg;
        func(m1), func(m2);
    } else if constexpr (size<T>() == 3) {
        const auto &[m1, m2, m3] = arg;
        func(m1), func(m2), func(m3);
    } else if constexpr (size<T>() == 4) {
        const auto &[m1, m2, m3, m4] = arg;
        func(m1), func(m2), func(m3), func(m4);
    }
}

int main() {
    struct X { std::string s{ " " }; }x;
    struct Y { double a{}, b{}, c{}, d{}; }y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto &member) {
        std::cout << member << ' ';
    };
    for_each_member(x, print);
    for_each_member(y, print);
}
