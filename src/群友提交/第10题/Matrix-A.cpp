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
        requires requires() { {T{ { (void(Is), Anything{}) }... } } -> std::same_as<T>; }
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
    requires std::is_aggregate_v<std::remove_cvref_t<T>>
using constructor_arity = detail::maximize< 0, Cap, detail::construct_searcher<std::remove_cvref_t<T>>::template result >;

template <typename T, std::size_t Cap = cap_max>
    requires std::is_aggregate_v<std::remove_cvref_t<T>>
constexpr std::size_t constructor_arity_v = constructor_arity<std::remove_cvref_t<T>, Cap>::value;


template<typename T, std::size_t Cap = cap_max>
    requires std::is_aggregate_v<std::remove_cvref_t<T>>
consteval std::size_t size() {
    return constructor_arity_v<std::remove_cvref_t<T>, Cap>;
}

template <typename T, typename Fn, std::size_t Cap = cap_max>
    requires std::is_aggregate_v<std::remove_cvref_t<T>>
void for_each_member(const T& agg, Fn&& fn) {
    constexpr auto size = constructor_arity_v<std::remove_cvref_t<T>, Cap>;
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
    else if constexpr (size == 5) {
        auto& [m0, m1, m2, m3, m4] = agg;
        fn(m0); fn(m1); fn(m2); fn(m3); fn(m4);
    }
    else {
        // todo
    }
}

/*
T 类型的对象的列表初始化的效果是：

1. 如果花括号初始化列表 ﻿包含指派初始化器列表，并且 T 不是引用类型，那么 T 必须是聚合类。由指派初始化器列表中的指派符组成的标识符序列必须是 T 的非静态数据成员组成的标识符序列的子序列。进行聚合初始化。
(C++20 起)
2. 如果 T 是聚合类且花括号初始化列表 ﻿在不含指派初始化器列表的情况下(C++20 起)拥有单个（可有 cv 限定的）相同类型或派生类型的元素，那么从该元素初始化对象（对于复制列表初始化为复制初始化，对于直接列表初始化为直接初始化）。
3. 否则，如果 T 是字符数组且初始化列表拥有单个类型适当的字符串字面量元素，那么照常从字符串字面量初始化数组。
4. 否则，如果 T 是聚合类型，那么进行聚合初始化。
5. 否则，如果花括号初始化列表 ﻿列表为空，且 T 是拥有默认构造函数的类类型，那么进行值初始化。
6. 否则，如果 T 是 std::initializer_list 的特化，那么该 T 对象会以如下方式初始化。
7. 否则，以两个阶段考虑 T 的构造函数：
7.1 检验所有接受 std::initializer_list 作为它的唯一实参，或如果剩余实参都具有默认值则为它的首个实参的构造函数，并通过重载决议与单个 std::initializer_list 类型的实参进行匹配。
7.2 如果上一个阶段未产生匹配，那么 T 的所有构造函数都参与针对由花括号初始化列表 ﻿的各元素所组成的实参集的重载决议，它会受到只允许非窄化转换的限制。如果这个阶段为复制列表初始化产生的最佳匹配是 explicit 构造函数，那么编译失败（注意：简单复制初始化中完全不考虑 explicit 构造函数）。
...
*/

/*
* 使用 { { detail::Anything{} } ... } 初始化成员的匹配顺序：
* 1. 不适用，无指派初始化器
* 2. 适用
* 3. 不适用
* 4. 适用，聚合初始化（如果为空类，会产生初始化失败，因为参数个数1，需要参数个数为0）
* 5. 不适用，花括号初始化列表不为空
* 6. 适用
* 7.1 适用，优先考虑接受 std::initializer_list 作为它的唯一实参的构造函数
* 7.2 适用，优先匹配到复制构造或移动构造，如果存在
* 其他：不可能到后续匹配
*/

/*
* 因此聚合类成员的适用范围
* 1. 基础类型
* 2. 聚合类型
* 3. 非聚合类型，但是拥有接受 std::initializer_list 作为它的唯一实参的构造函数
* 4. 非聚合类型，不存在接受 std::initializer_list 作为它的唯一实参的构造函数，但存在移动或复制构造函数，且不存在其他接受单一实参的构造函数
*/

/*
* 注： 适用 mqb方案 表示使用  { detail::Anything{}... }  初始化成员
* 
* 已知特殊情况
* 1. 成员为空类且聚合类型对象，不适用，因为参数过多（mqb方案适用这种情况，因为会直接转为对应的对象）
* 2. 存在空基类的聚合类型对象，不适用，因为基类需要一个"{}"进行初始化（mqb方案也不使用，存在一个Anything用于初始化基类）（详见花括号消除）
*/

int main() {
    auto print = [](const auto& member) {
        if constexpr (!requires { std::cout << member; }) {
            std::cout << "[" << &member << "] ";
        }
        else {
            std::cout << member << ' ';
        }
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
    {
        std::cout << "------------\n";
        struct A { int a{ 1 }; double b[2]{ 2.3, 3.4 }; std::string c{ "5" }; } a;
        std::cout << size<A>() << '\n';
        for_each_member(a, print);
        std::cout << "\n";
    }
    {
        std::cout << "------------\n";
        struct A { int a{ 1 }; double b[2]{ 2.3, 3.4 }; std::string c{ "5" }; } a[3];
        std::cout << size<decltype(a)>() << '\n';
        for_each_member(a, print);
        std::cout << "\n";
    }
    {
        std::cout << "------------\n";
        struct A { int a{ 1 }; std::array<double, 2> b{ 2.3, 3.4 }; std::string c{ "5" }; } a;
        std::cout << size<decltype(a)>() << '\n';
        for_each_member(a, print);
        std::cout << "\n";
    }
    // test const or reference
    {
        struct A{ int a{ 1 }; double b[2]{ 2.3, 3.4 }; std::string c{ "5" }; };
        {
            std::cout << "------------\n";
            const A a[3];
            std::cout << size<decltype(a)>() << '\n';
            for_each_member(a, print);
            std::cout << "\n";
        }
        {
            std::cout << "------------\n";
            const A& a{};
            std::cout << size<decltype(a)>() << '\n';
            for_each_member(a, print);
            std::cout << "\n";
        }
        {
            std::cout << "------------\n";
            A&& a{};
            std::cout << size<decltype(a)>() << '\n';
            for_each_member(a, print);
            std::cout << "\n";
        }
    }
    {
        {
            std::cout << "------------\n";
            struct A { int a{ 1 }; double b[2][1]{ {2.3}, {4.5} }; std::string c{ "5" }; } a;
            std::cout << size<decltype(a)>() << '\n';
            for_each_member(a, print);
            std::cout << "\n";
        }
        {
            std::cout << "------------\n";
            struct A { int a{ 1 }; std::array<std::array<double, 1>, 2> b{ {2.3, 4.5} }; std::string c{ "5" }; } a;
            std::cout << size<decltype(a)>() << '\n';
            for_each_member(a, print);
            std::cout << "\n";
        }
    }
    {
        // error
        // struct A { int a{ 1 }; std::string &c; };
        // {
        //     std::cout << "------------\n";
        //     A a{ {}, detail::Anything{} };
        //     std::cout << size<decltype(a)>() << '\n';
        //     for_each_member(a, print);
        // }
    }
}
