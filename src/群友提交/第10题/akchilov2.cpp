#include<type_traits>
#include<iostream>
#include<array>
#include<numeric>
#include<tuple>

//需要C++20以上
//msvc下可以编译
//这个程序的原理是用std::is_constructible猜测类的成员类型
//然后使用偏移量来访问类的数据成员
namespace my_utilities
{
    struct inits
    {
        template<class T> operator T();
    };

    //这个类型用来试探某种类型是不是类的第N个成员类型
    //由于std::is_constructible隐式转换也会true，所以这里禁止隐式转换
    //以求结果精确
    template<class T> struct convert_forbid
    {
        using type = T;
        template<
            class U,
            class En = std::enable_if_t<std::is_same_v<T, U>>
        > operator U();
    };
    //计算成员数量
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
            (std::is_constructible_v<T, First, rest...> ? cnt : cnt - 1)>
            ::value;
    };
    //工具类，选择类型分支
    //本来我想用type_traits里面的，不熟悉就没用
    template<bool cond, class A, class B> struct select_impl
    {
        using type = A;
    };

    template<class A, class B> struct select_impl<false, A, B>
    {
        using type = B;
    };
    template<bool b, class A, class B> using select_branch = typename select_impl<b, A, B>::type;

    template<class T, class TL> struct add_front {};
    template<class T, template<class...> class TL, class...Args> struct add_front<T, TL<Args...>>
    {
        using type = TL<T, Args...>;
    };
  
    template<size_t N, class T, class TL> struct gen_list
    {
        using type = TL;
    };

    template<size_t N, template<class...> class TL, class T, class ...Args> struct gen_list<N, T, TL<Args...>>
    {
        using type = typename gen_list<N - 1, T, TL<Args..., T>>::type;
    };

    template<class T, class TL> struct gen_list<0, T, TL> { using type = TL; };

    template<size_t N, class pre, class bck, class T> struct replace_at
    {
    };

    template<size_t N, template<class...> class pre, template<class...> class bck, class RP, class bck_first, class ...preargs, class... bckargs>
    struct replace_at<N, pre<preargs...>, bck<bck_first, bckargs...>, RP>
    {
        using type = typename replace_at<N - 1, pre<preargs..., bck_first>, bck<bckargs...>, RP>::type;
    };

    template<template<class...> class pre, template<class...> class bck, class RP, class bck_first, class ...preargs, class... bckargs>
    struct replace_at<0, pre<preargs...>, bck<bck_first, bckargs...>, RP>
    {
        using type = pre<preargs..., RP, bckargs...>;
    };

    template<class TL, template<class...> class Name> struct as_ {};
    template<template<class...>class TL, template<class...>class Name, class ...Args> struct as_<TL<Args...>, Name>
    {
        using type = Name<Args...>;
    };
    template<class ...> struct tl {};

    //在列表种选择元素
    //懒得写了，骗一下这个tuple_element_t
    template<size_t N, class TL> struct select_element { using type = std::tuple_element_t<N, typename as_<TL, std::tuple>::type>; };

}
using namespace my_utilities;



template<class T> constexpr size_t size()
{
    return count_size<false, std::decay_t<T>, tl<inits, inits, inits, inits, inits, inits, inits, inits, inits, inits>, 10>::value;
}


//工具类，测试T的N个成员是否为Mem_type
template<size_t N, class T, class Mem_type> struct is_constructible_at
{
    using construct_list = typename gen_list<size<T>(), inits, tl<T>>::type;
    using chk_constructible = typename replace_at<N + 1, tl<>, construct_list, convert_forbid<Mem_type>>::type;

    using constructible_type = typename as_<chk_constructible, std::is_constructible>::type;
    static constexpr bool value = constructible_type::value;

};
//工具类，用一个列表mem_type_list逐个测试is_constructilbe_at，得到T的第N个成员的类型
template<bool constructible_, size_t N, class T, class mem_type_list> struct constructible_at_try {};
template<bool constructible_, size_t N, class T, template<class...> class mem_type_list, class first, class ...Args>
struct constructible_at_try<constructible_, N, T, mem_type_list<first, Args...>>
{
    static constexpr bool cstible = is_constructible_at<N, T, first>::value;
    using type = typename constructible_at_try<cstible,
        N,
        T,
        select_branch<cstible, mem_type_list<first, Args...>, mem_type_list<Args...>
        >
    >::type;
};

template<size_t N, class T, template<class...> class mem_type_list, class first, class ...Args> struct constructible_at_try<true, N, T, mem_type_list<first, Args...>>
{
    using type = first;
};
//工具类，生成成员列表
template<size_t N, class T, class TL, class saved> struct make_construct_list
{};

template<size_t N, class T, template<class...> class saved, class TL, class ...Args> struct make_construct_list<N, T, TL, saved<Args...>>
{
    using current_type = typename constructible_at_try<false, N, T, TL>::type;
    using type = typename make_construct_list<N - 1, T, TL, saved<current_type, Args...>>::type;
};

template<class T, class TL, class saved> struct make_construct_list<0, T, TL, saved>
{
    using current_type = typename constructible_at_try<false, 0, T, TL>::type;
    using type = typename add_front<current_type, saved>::type;
};
//计算偏移
template<size_t pack_size, template<class...> class TL, class ...Typs> constexpr auto make_offset_list(TL<Typs...>)
{
    return std::array{ (sizeof(Typs) % pack_size ? sizeof(Typs) / pack_size * pack_size + pack_size : sizeof(Typs))... };
}


template<size_t pack_size, class T, class possibilities> class offset_pointer
{
    using construct_list = make_construct_list<size<T>() - 1, T, possibilities, tl<>>::type;
public:
    offset_pointer(T a) noexcept : baseptr(reinterpret_cast<unsigned char*>(&a)) {}
    template<size_t I> typename select_element<I, construct_list>::type get() const
    {
        if constexpr (I == 0) {
            return *(reinterpret_cast<std::add_pointer_t<typename select_element<0, construct_list>::type>>(baseptr));
        }
        else
        {
            return *(
                reinterpret_cast<std::add_pointer_t<typename select_element<I, construct_list>::type>>(
                    baseptr + std::accumulate(member_offset.begin() + 1, member_offset.begin() + 1 + I, 0)
                    )
                );
        }
    }
private:
    unsigned char* baseptr;
    std::array<size_t, size<T>()>
        member_offset{
        make_offset_list<pack_size>(make_construct_list<size<T>() - 1, T, possibilities, tl<>>::type())
    };
};

//理论上可以算内存对齐，但是已经太长了，不想写那个了
template<class T, class F, size_t ...I> void for_each_member_impl(T&& t, F&& f, std::index_sequence<I...>)
{
    //有点上帝视角了，因为已经看到了所有的数据类型
    using possible_types = tl<int, std::string, double>;
    offset_pointer<alignof(T), T, possible_types> op{ t };
    (f(op.get<I>()), ...);
}

template<class T, class F> constexpr void for_each_member(T&& t, F&& f)
{
    for_each_member_impl(std::move(t), std::move(f), std::make_index_sequence<size<T>()>{});
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
