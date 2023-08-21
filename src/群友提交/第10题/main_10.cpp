#include <iostream>
#include <tuple>
#include <type_traits>

template <unsigned I>
struct tag : tag<I - 1>
{
};

template <>
struct tag<0>
{
};

// tag<I> 实际定义了一个编译期的整数, 类型是tag的一个实例，I为对应整数
// 用到这个类的目的，是为了 size<Y> 的时候
// 尽管有4个成员，但实际0 ~ 3的size_版本也可以选，通过tag<I> 来避免

template <typename T>
constexpr auto size_(tag<4>) -> decltype(T{{}, {}, {}, {}}, 0u)
{
    return 4u;
    // decltype(T{{}, {}, {}, {}}, 0u) 的类型是 decltype(0u) -> unsigned int 或者 模板匹配失败
    // 下述相同
}

template <typename T>
constexpr auto size_(tag<3>) -> decltype(T{{}, {}, {}}, 0u)
{
    return 3u;
}

template <typename T>
constexpr auto size_(tag<2>) -> decltype(T{{}, {}}, 0u)
{
    return 2u;
}

template <typename T>
constexpr auto size_(tag<1>) -> decltype(T{{}}, 0u)
{
    return 1u;
}

template <typename T>
constexpr auto size_(tag<0>) -> decltype(T{}, 0u)
{
    return 0u;
}

// 以上5个函数通过SFINAE来实现5个版本

template <typename T>
constexpr size_t size()
{
    static_assert(std::is_aggregate_v<T>);
    return size_<T>(tag<4>{});  // 从4开始往下试 （还没完全理解，感受下）
}

template <typename T, typename F>
void for_each_member(T const& v, F f)
{
    static_assert(std::is_aggregate_v<T>);

    if constexpr (size<T>() == 4u)
    {
        const auto& [m0, m1, m2, m3] = v;
        f(m0);
        f(m1);
        f(m2);
        f(m3);
    }
    else if constexpr (size<T>() == 3u)
    {
        const auto& [m0, m1, m2] = v;
        f(m0);
        f(m1);
        f(m2);
    }
    else if constexpr (size<T>() == 2u)
    {
        const auto& [m0, m1] = v;
        f(m0);
        f(m1);
    }
    else if constexpr (size<T>() == 1u)
    {
        const auto& [m0] = v;
        f(m0);
    }
}
// 编译期 if constexpr来通过size得到个数后，用[a,b,c,d]解包aggregate类型

// 总体来说确实完全抄的 https://akrzemi1.wordpress.com/2020/10/01/reflection-for-aggregates/
// 这里至少用到了3个trick 确实很难 我不看文章基本不可能解决
// 阅读这种代码还是有助于理解模板的 不过普通人应该会直接用boost
// 真正上手完全实现的人应该是开发工业级库的同学

int main()
{
    struct X
    {
        std::string s{" "};
    } x;
    struct Y
    {
        double a{}, b{}, c{}, d{};
    } y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto& member) { std::cout << member << ' '; };
    for_each_member(x, print);
    for_each_member(y, print);
}
