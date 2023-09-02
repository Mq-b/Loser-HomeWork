#include <type_traits>
#include <iostream>

// 可以自动转换为任意的类型的类型
struct any_type
{
    template<typename T>
    operator T();
};

// 用于终止偏特化，所以答案是此时用于初始化的类型数量减一
// typename用于偏特化,或者说编译期分发,是一个哑类型。
template<typename T,typename = void,typename... Ts>
struct count_member
{
    constexpr static std::size_t value = sizeof...(Ts) - 1;
};

// 偏特化
// 在当前可以聚合初始化时，用于向Ts中加入1个任意类型继续试探，直至不可以为止
template<typename T,typename... Ts>
struct count_member<T,std::void_t<decltype(T{ Ts{}... })>,Ts...>
{
    // 实例化下一个试探值
    constexpr static std::size_t value = count_member<T,void,Ts...,any_type>::value;
};

template<typename T>
    requires std::is_aggregate_v<T>
constexpr std::size_t size()
{
    return count_member<T>::value;
}

// 打表
template <typename T, typename F>
    requires (size<T>() < 5u)
void for_each_member(T t, F f)
{
    if constexpr (size<T>() == 1u)
    {
        auto [m0] = t;

        f(m0);
    }
    else if constexpr (size<T>() == 2u)
    {
        auto [m0, m1] = t;

        f(m0);
        f(m1);
    }
    else if constexpr (size<T>() == 3u)
    {
        auto [m0, m1, m2] = t;

        f(m0);
        f(m1);
        f(m2);
    }
    else if constexpr (size<T>() == 4u)
    {
        auto [m0, m1, m2, m3] = t;

        f(m0);
        f(m1);
        f(m2);
        f(m3);
    }

    // 0个元素
}

int main(){
    struct X { std::string s{ " "  }; } x;
    struct Y { double a{}, b{}, c{}, d{}; } y;

    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto& member) {
        std::cout << member << ' ';
    };

    for_each_member(x, print);
    for_each_member(y, print);

    return 0;
}