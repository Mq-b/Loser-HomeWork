#include<iostream>

#define BY_NAME(...) \
[](auto &&...args) {\
    return __VA_ARGS__(std::forward<decltype(args)>(args)...);\
}\

template<typename F, class...Args>
auto foo(F f, Args&&...args) {
    return f(std::forward<Args>(args)...);
}

int main() {
    const auto result = foo(BY_NAME(std::min), 2, 3);
    std::cout << result << '\n';
}
