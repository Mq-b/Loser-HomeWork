#include<type_traits>
#include<iostream>
#include<string>

//任意类
struct init
{
    template<class T> operator T();
};

//从一个用一个递减的类型列表尝试能不能std::is_constructible_v
template<bool, class T, class construct_list, size_t cnt> struct count_size
{
    static constexpr size_t value = cnt;
};

template<class T, template<class ...> class construct_list, size_t cnt, class First, class ...rest>
struct count_size<false, T, construct_list<First, rest...>, cnt>
{
    static constexpr size_t value =
        count_size<
        std::is_constructible_v<T, First, rest...>,
        T,
        construct_list<rest...>,
       (std::is_constructible_v<T, First, rest...>? cnt : cnt - 1)>
        ::value;
};
template<class ...> struct tl {};

template<class T> constexpr size_t size()
{
    return count_size<false, std::decay_t<T>, tl<init, init, init, init>, 4>::value;
}

template<class T, class F> constexpr void for_each_member(T&& i, F&& f)
{
    if constexpr (size<T>() == 4u)
    {
        auto&& [a, b, c, d] = i;
        f(a), f(b), f(c), f(d);
    }
    if constexpr (size<T>() == 3u)
    {
        auto&& [a, b, c] = i;
        f(a), f(b), f(c);
    }
    if constexpr (size<T>() == 2u)
    {
        auto&& [a, b] = i;
        f(a), f(b);
    }
    if constexpr (size<T>() == 1u)
    {
        auto&& [a] = i;
        f(a);
    }
}



int main() {
    struct X { std::string s{ " " }; }x;
    struct Y { double a{}, b{}, c{}, d{}; }y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto& member) {
        std::cout << member << ' ';
        };
    for_each_member(x, print);
    for_each_member(y, print);
}
