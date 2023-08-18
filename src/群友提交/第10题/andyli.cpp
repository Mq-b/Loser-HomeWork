#include <iostream>

namespace impl {
    struct Helper {
        template <typename T>
        operator T();
    };
} // namespace impl

template <typename T>
    requires std::is_aggregate_v<T>
constexpr size_t size(auto&&... args) {
    if constexpr (requires { T{args...}; })
        return size<T>(args..., impl::Helper{});
    return sizeof...(args) - 1;
}
template <typename T>
    requires std::is_aggregate_v<T>
void for_each_member(const T& v, auto&& f) {
    if constexpr (size<T>() == 1u) {
        auto&& [a] = v;
        f(a);
    }
    if constexpr (size<T>() == 2u) {
        auto&& [a, b] = v;
        f(a), f(b);
    }
    if constexpr (size<T>() == 3u) {
        auto&& [a, b, c] = v;
        f(a), f(b), f(c);
    }
    if constexpr (size<T>() == 4u) {
        auto&& [a, b, c, d] = v;
        f(a), f(b), f(c), f(d);
    }
}
int main() {
    struct X {
        std::string s{" "};
    } x;
    struct Y {
        double a{}, b{}, c{}, d{};
    } y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';
    auto print = [](const auto& member) {
        std::cout << member << ' ';
    };
    for_each_member(x, print);
    for_each_member(y, print);
}
