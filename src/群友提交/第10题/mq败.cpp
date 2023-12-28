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

struct Pos {
    int x;
    int y;
    int z;
};

struct Agg {
    int i;
    Pos pos;
    int arr[4];
    std::string str;
    std::vector<int> vec;
    float f;
};

auto main() ->int {
    std::cout << size<Agg> << std::endl;
    // 遍历成员那个就不写了, 😊
    return 0;
}
