#include <iostream>
#include <type_traits>
#include <vector>

namespace detail {
    template<typename...>
    constexpr bool always_false = false;

    template<typename T>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    template<std::size_t N>
    using size_constant = std::integral_constant<std::size_t, N>;

    struct enable_as_any_type_helper {
        template<typename T>
        operator T () noexcept;
    };

    template<std::size_t I>
    using enable_as_any_type = enable_as_any_type_helper;

    template<typename Aggregate, typename IndexSequence, typename = void>
    struct test_constructible_helper : std::false_type {};

    template<typename Aggregate, std::size_t... Idx>
    struct test_constructible_helper<Aggregate, std::index_sequence<Idx...>, std::void_t<decltype(Aggregate{{std::declval<enable_as_any_type<Idx>>()}...})>> : std::true_type {};

    template<typename Aggregate, std::size_t N>
    struct test_constructible : test_constructible_helper<Aggregate, std::make_index_sequence<N>> {};

    template<std::size_t N, typename Aggregate> 
    struct size_helper : std::conditional_t<test_constructible<Aggregate, N+1>::value, size_helper<N+1, Aggregate>, size_constant<N>> {};

    template<typename T, typename = void>
    struct size {
        static_assert(always_false<T>, "type `T` must be an aggregate-type!");
    };

    template<typename Aggregate>
    struct size<Aggregate, std::enable_if_t<std::is_aggregate_v<Aggregate>>> : size_helper<0, Aggregate> {};
}

template<typename Aggregate>
constexpr std::size_t size = detail::size<Aggregate>::value;

/*
限制:
要求聚合体的非静态数据成员的类型是以下类型之一: 
    1) 标量
    2) 聚合体
    3) 拥有第一个形参的类型为std::initializer_list, 其余形参(若存在)有默认值的非explicit的public构造函数的类
*/

struct Pos {
    int x;
    int y;
    int z;
};

struct A {
    A(std::initializer_list<int>) noexcept {}
};

struct Agg {
    int i;                // 标量
    Pos pos;              // 聚合体
    int arr4[4];          // 聚合体
    std::string str;      // 拥有第一个形参的类型为std::initializer_list, 其余形参有默认值的构造函数的类
    std::vector<int> vec; // 拥有第一个形参的类型为std::initializer_list, 其余形参有默认值的构造函数的类
    int arr6[6];          // 聚合体
    float f;              // 标量
    A a;                  // 拥有第一个形参的类型为std::initializer_list的类
};

auto main() ->int {
    std::cout << size<Agg> << std::endl; // 8
    return 0;
}
