#include <utility>
#include <type_traits>
#include <stddef.h>

namespace detail {

template <typename T, typename... Args>
struct is_constructable {
    template <typename G, typename... Ts>
    static constexpr auto check(int)
        -> decltype(T{std::declval<Ts>()...}, std::true_type{});

    template <typename G, typename... Ts>
    static constexpr std::false_type check(...);

    using type = decltype(check<T, Args...>(0));
};

} // namespace detail

template <typename T, typename... Args>
static constexpr inline bool is_constructable_v =
    detail::is_constructable<T, Args...>::type::value;

struct E {
    template <typename T>
    operator T();
};

template <typename T, typename... Args>
constexpr size_t size() {
    if constexpr (is_constructable_v<T, E, Args...>) {
        return size<T, Args..., E>();
    } else {
        return sizeof...(Args);
    }
}

template <typename T, typename Fn>
void for_each_member(const T &e, Fn f) {
    constexpr auto n = size<T>();
    static_assert(n <= 4);
    if constexpr (n == 1) {
        const auto &[_1] = e;
        f(_1);
    } else if constexpr (n == 2) {
        const auto &[_1, _2] = e;
        f(_1);
        f(_2);
    } else if constexpr (n == 3) {
        const auto &[_1, _2, _3] = e;
        f(_1);
        f(_2);
        f(_3);
    } else if constexpr (n == 4) {
        const auto &[_1, _2, _3, _4] = e;
        f(_1);
        f(_2);
        f(_3);
        f(_4);
    }
}

#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    struct X {
        std::string s{"Hello World!"};
    } x;

    struct Y {
        double a{11.4}, b{51.4}, c{19.19}, d{870};
    } y;

    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto &member) {
        std::cout << member << ' ';
    };
    for_each_member(x, print);
    for_each_member(y, print);

    return 0;
}
