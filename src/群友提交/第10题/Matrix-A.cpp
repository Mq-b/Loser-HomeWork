#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
//#include <format>
#include <stdexcept>
#include <new>
#include <array>
#include <utility> 
#include <tuple> 
#include <ranges> 
#include <cstdint> 

// 参考文章 https://bitwizeshift.github.io/posts/2021/03/21/reflecting-over-members-of-an-aggregate/

namespace detail {

    struct Anything {
        template <typename T>
        operator T() const; // 不需要定义该函数
    };

    template <typename T, typename Is>
    struct is_aggregate_constructible_from_n_impl 
        : std::false_type {};

    template <typename T, std::size_t...Is>
        requires requires() { {T{ (void(Is), Anything{})... } } -> std::same_as<T>; }
    struct is_aggregate_constructible_from_n_impl<T, std::index_sequence<Is...>> 
        : std::true_type {};

    template <typename T, std::size_t N>
    using is_aggregate_constructible_from_n = detail::is_aggregate_constructible_from_n_impl<T, std::make_index_sequence<N>>;

    template <typename T>
    struct construct_searcher {
        template<std::size_t N>
        using result = is_aggregate_constructible_from_n<T, N>;
    };

    // 二分查找
    template <std::size_t Min, std::size_t Range, template <std::size_t N> class target>
    struct maximize
        : std::conditional_t <
        maximize<Min, Range / 2, target>{} == (Min + Range / 2) - 1,
        maximize<Min + Range / 2, (Range + 1) / 2, target>,
        maximize<Min, Range / 2, target>
        > {};
    template <std::size_t Min, template <std::size_t N> class target>
    struct maximize<Min, 1, target>
        : std::conditional_t <
        target<Min>{},
        std::integral_constant<std::size_t, Min>,
        std::integral_constant<std::size_t, Min - 1>
        > {};
    template <std::size_t Min, template <std::size_t N> class target>
    struct maximize<Min, 0, target>
        : std::integral_constant<std::size_t, Min - 1>
    {};
} // namespace detail


constexpr std::size_t cap_max = 0xff; // 假设不超过 0xff 个元素，则递归深度不超过 log2(0xff) ，约为 8

template <typename T, std::size_t Cap = cap_max>
    requires std::is_aggregate_v<std::decay_t<T>>
using constructor_arity = detail::maximize< 0, Cap, detail::construct_searcher<T>::template result >;

template <typename T, std::size_t Cap = cap_max>
    requires std::is_aggregate_v<std::decay_t<T>>
constexpr std::size_t constructor_arity_v = constructor_arity<T, Cap>::value;


template<typename T, std::size_t Cap = cap_max>
    requires std::is_aggregate_v<std::decay_t<T>>
consteval std::size_t size() {
    return constructor_arity_v<std::decay_t<T>, Cap>;
}

template <typename T, typename Fn, std::size_t Cap = cap_max>
    requires std::is_aggregate_v<std::decay_t<T>>
void for_each_member(const T& agg, Fn&& fn) {
    constexpr auto size = constructor_arity_v<std::decay_t<T>, Cap>;
    if constexpr (size == 0) {
        // nothing
    }
    else if constexpr (size == 1) {
        auto& [m0] = agg;
        fn(m0);
    }
    else if constexpr (size == 2) {
        auto& [m0, m1] = agg;
        fn(m0); fn(m1);
    }
    else if constexpr (size == 3) {
        auto& [m0, m1, m2] = agg;
        fn(m0); fn(m1); fn(m2);
    }
    else if constexpr (size == 4) {
        auto& [m0, m1, m2, m3] = agg;
        fn(m0); fn(m1); fn(m2); fn(m3);
    }
    else {
        // TODO 更大的size
    }
}

int main() {
    auto print = [](const auto& member) {
        std::cout << member << ' ';
        };
    {
        std::cout << "------------\n";
        struct Null {  } n;
        std::cout << size<Null>() << '\n';
        for_each_member(n, print);
        std::cout << "\n";
    }
    {
        std::cout << "------------\n";
        struct X { std::string s{ "*" }; } x;
        std::cout << size<X>() << '\n';
        for_each_member(x, print);
        std::cout << "\n";
    }
    {
        std::cout << "------------\n";
        struct Y { double a{}, b{}, c{}, d{}; } y;
        std::cout << size<Y>() << '\n';
        for_each_member(y, print);
        std::cout << "\n";
    }
    {
        std::cout << "------------\n";
        struct Z { int a{ 1 }; double b{ 2.0 }; std::string c{ "3" }; char d{ '4' }; } z;
        std::cout << size<Z>() << '\n';
        for_each_member(z, print);
        std::cout << "\n";
    }
    //{
    //    // error
    //    std::cout << "------------\n";
    //    struct A { int a{ 1 }; double b[2]{ 2.3, 3.4 }; std::string c{ "5" }; } a;
    //    std::cout << size<A>() << '\n';
    //    for_each_member(a, print);
    //    std::cout << "\n";
    //}
}

