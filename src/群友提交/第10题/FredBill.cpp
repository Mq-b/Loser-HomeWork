#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

struct any_type {
    template <typename T> operator T() const;
};
template <typename T>
    requires std::is_aggregate_v<std::remove_cvref_t<T>>
constexpr size_t size(auto&&... args) {
    using U = std::remove_cvref_t<T>;
    if constexpr (requires { U{args...}; }) return size<U>(any_type(), args...);
    return sizeof...(args) - 1;
}
template <typename T, typename F>
    requires std::is_aggregate_v<std::remove_cvref_t<T>>
void for_each_member(T&& x, F&& f) {
    auto for_each_args = [&f](auto&&... args) { (f(args), ...); };
    constexpr std::size_t size_v = size<T>();
    if constexpr (size_v == 0) return;
#define FOR_EACH_MEMBER_IMPL(sz, ...)  \
    else if constexpr (size_v == sz) { \
        auto&& [__VA_ARGS__] = x;      \
        for_each_args(__VA_ARGS__);    \
    }
    FOR_EACH_MEMBER_IMPL(1, v0)
    FOR_EACH_MEMBER_IMPL(2, v0, v1)
    FOR_EACH_MEMBER_IMPL(3, v0, v1, v2)
    FOR_EACH_MEMBER_IMPL(4, v0, v1, v2, v3)
#undef FOR_EACH_MEMBER_IMPL
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

    auto print = [](const auto& member) { std::cout << member << ' '; };
    for_each_member(x, print);
    for_each_member(y, print);
}