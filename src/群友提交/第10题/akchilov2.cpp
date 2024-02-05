#include<type_traits>
#include<tuple>
#include<iostream>
namespace possibilities
{
    //需要C++20以上
    //测试了MSVC latest, clang16, gcc13
    //原理是用std::is_constructible猜测类的成员类型
    //注意：需要给出类中可能出现的所有类型，但是不一定是精确的，可以多猜点，但是不能缺少，缺少会编译不过
    namespace possibilities_utilities
    {
        struct inits
        {
            template<class T> operator T();
        };
        //这个类型用来试探某种类型是不是类的第N个成员类型
        //由于std::is_constructible隐式转换也会true，所以这里禁止隐式转换以求结果精确
        template<class T> struct convert_forbid
        {
            using type = T;
            template<class U, class En = std::enable_if_t<std::is_same_v<T, U>>> operator U();
        };
        //计算成员数量,生成构造列表
        template<bool, class T, class construct_list, std::size_t cnt> struct count_size {};
        template<class T, template<class...> class construct_list, std::size_t cnt, class ...types>
        struct count_size<true, T, construct_list<types...>, cnt>
        {
            static constexpr std::size_t value = cnt;
            using CTL = construct_list<T, types...>;
        };
        template<class T, template<class ...> class construct_list, std::size_t cnt, class First, class ...rest>
        struct count_size<false, T, construct_list<First, rest...>, cnt>
        {
            using next = count_size<std::is_constructible_v<T, First, rest...>, T, std::conditional_t<
                std::is_constructible_v<T, First, rest...>, construct_list<First, rest...>, construct_list<rest...>>,
                (std::is_constructible_v<T, First, rest...> ? cnt : cnt - 1)>;
            static constexpr std::size_t value = next::value;
            using CTL = typename next::CTL;
        };
        //工具类，用于替换构造列表中的元素
        template<std::size_t N, class pre, class bck, class T> struct replace_at {};
        template<std::size_t N, template<class...> class pre, template<class...> class bck, class RP, class bck_first, class ...preargs, class... bckargs>
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
        //在列表中选择元素
        template<std::size_t N, class TL> struct select_element { using type = std::tuple_element_t<N, typename as_<TL, std::tuple>::type>; };
    }
    using namespace possibilities_utilities;

    template<class ...> struct tl {};

    template<class T> using count_size_type = count_size<false, std::decay_t<T>, tl<inits, inits, inits, inits, inits, inits, inits, inits, inits, inits, inits, inits, inits, inits, inits>, 15>;
    template<class T> constexpr std::size_t size() {
        return count_size_type<T>::value;
    }
    //工具类，测试T的N个成员是否为Mem_type
    template<std::size_t N, class T, class Mem_type> struct is_constructible_at
    {
        using construct_list = typename count_size_type<T>::CTL;
        using chk_constructible = typename replace_at<N + 1, tl<>, construct_list, convert_forbid<Mem_type>>::type;
        using constructible_type = typename as_<chk_constructible, std::is_constructible>::type;
        static constexpr bool value = constructible_type::value;

    };
    //工具类，用一个列表mem_type_list逐个测试is_constructilbe_at，得到T的第N个成员的类型
    template<bool constructible_, std::size_t N, class T, class mem_type_list> struct constructible_at_try {};
    template<bool constructible_, std::size_t N, class T, template<class...> class mem_type_list, class first, class ...Args>
    struct constructible_at_try<constructible_, N, T, mem_type_list<first, Args...>>
    {
        static constexpr bool cstible = is_constructible_at<N, T, first>::value;
        using type = typename constructible_at_try<cstible, N, T,
            std::conditional_t<cstible, mem_type_list<first, Args...>, mem_type_list<Args...>>>::type;
    };

    template<std::size_t N, class T, template<class...> class mem_type_list, class first, class ...Args>
    struct constructible_at_try<true, N, T, mem_type_list<first, Args...>> { using type = first; };
    //工具类，生成成员列表
    template<std::size_t N, class T, class TL, class saved> struct make_construct_list {};
    template<class T, template<class...> class TL, template<class...> class saved, class ...Args, class ...savedArgs>
    struct make_construct_list<0, T, TL<Args...>, saved<savedArgs...>>
    {
        using current_type = typename constructible_at_try<false, 0, T, TL<Args...>>::type;
        using type = saved<current_type, savedArgs...>;
    };
    template<std::size_t N, class T, template<class...> class saved, class TL, class ...Args> struct make_construct_list<N, T, TL, saved<Args...>>
    {
        using current_type = typename constructible_at_try<false, N, T, TL>::type;
        using type = typename make_construct_list<N - 1, T, TL, saved<current_type, Args...>>::type;
    };

    template<class T, class Poss> using possibility = typename make_construct_list<possibilities::size<T>() - 1, T, Poss, tl<>>::type;
}

namespace offset_pointer {
    template<typename structT, typename T>
 T& read_from_align_offset(structT* base_ptr, const std::size_t offset)
 {
     return *(reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base_ptr) + offset));
 }
 //编译时计算
 //offset_ptr模拟成员在结构体中的分布， advance：跨越一个指定的size， forward_offset:寻找下一个成员的首地址
 template<std::size_t pack_size, std::size_t layer_offset, std::size_t offset> struct align_offset_ptr
 {
     static_assert(pack_size <= 8, "An alignment size exceeds 8 is not tested!");
     static constexpr std::size_t layer = layer_offset;//alignment layer as base
     static constexpr std::size_t value = offset;//当前层偏移指针
     template<std::size_t lay> struct advance_layer
     {
         using type = align_offset_ptr<pack_size, layer + lay, 0>;
     };
     template<bool can_be_placed, std::size_t seek_offset> struct seek_impl
     {
         using type = align_offset_ptr<pack_size, layer, value>;
     };
     template<std::size_t seek_offset> struct seek_impl<false, seek_offset>
     {
         using type = typename std::conditional_t<
             (value + seek_offset > pack_size),
             typename advance_layer<1>::type,
             align_offset_ptr<pack_size, layer_offset, value + 1>
             >::template seek<seek_offset>;
     };
     template<std::size_t seek_offset> using seek = typename seek_impl<value % seek_offset == 0,seek_offset>::type;

     template<std::size_t advance_offset> struct advance_impl
     {
         using type = std::conditional_t<(value + advance_offset < pack_size),
             align_offset_ptr<pack_size, layer, value + advance_offset>,
             std::conditional_t<
             static_cast<bool>(advance_offset% pack_size),
             typename advance_layer<advance_offset / pack_size + 1>::type,
             typename advance_layer<advance_offset / pack_size>::type>
         >;
     };
     template<std::size_t advance_offset> using advance = typename advance_impl<advance_offset>::type;
     template<class structT, class T> 
     static T& get(structT* base_ptr)
     {
         using seek_ptr = seek<sizeof(T)>;
         return read_from_align_offset<structT, T>(base_ptr, seek_ptr::layer * pack_size + seek_ptr::value);
     }
 };
 //递归迭代，实现类似std::get的方法
 template<std::size_t N, class offset_pointer, class TL> struct offset_iter {};
 template<std::size_t N, class offset_pointer, template<class ...> class TL, class first, class ...rest>
 struct offset_iter<N, offset_pointer, TL<first, rest...>>
 {
     using recurse_type = offset_iter<N - 1, 
         typename offset_pointer
         ::template seek<sizeof(first)>
         ::template advance<sizeof(first)>, TL<rest...>>;
     using iter_type = typename recurse_type::iter_type;
     using type = typename recurse_type::type;

 };
 template<class offset_pointer, template<class...> class TL, class first, class ...rest>
 struct offset_iter<0, offset_pointer, TL<first, rest...>>
 {
     using type = first;
     using iter_type = offset_pointer;
 };
 //封装成函数
 template<std::size_t I, class CTL, class T> auto& get_member(T* x)
 {
     using types_iter = offset_iter<I, align_offset_ptr<alignof(T), 0, 0>, CTL>;

     return types_iter::iter_type::template get<T, typename types_iter::type>(x);
 }
}

using namespace possibilities;
using namespace offset_pointer;
using possible = tl<int, std::string, double, char, int, short>;

template<class T, class F, size_t ...I> constexpr void for_each_member_impl(T&& x, F&& f, std::index_sequence<I...>)
{
    ((f(get_member<I, possibility<T, possible>>(&x))), ...);
}

template<class T, class F> constexpr void for_each_member(T&& x, F&& f)
{
    for_each_member_impl(std::move(x), std::move(f), std::make_index_sequence<size<T>()>{});
}

int main() {
   struct X
{
    char aaa{ 'A' };
    int aaaak{ 100 };
    std::string str{ "hello X!" };
    char cc{ 'k' };
    int a{ 10 };
    char c{ 'l' };
    short r{ 123 };
    char e{ 'u' };
    short rr{ 889 };
    int ac{ 19 };
    double b{ 2.33 };
    std::string str2{ "hello X again!" };
    double bbg{ 4.55 };
} x{};    struct Y { double a{}, b{}, c{}, d{}; }y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto& member) {
        std::cout << member << ' ';
        };
    for_each_member(x, print);
    for_each_member(y, print);
}
