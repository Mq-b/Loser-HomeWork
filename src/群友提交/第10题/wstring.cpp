#include <iostream>
struct V
{
    template<class T>
    operator T(){}
};
template<class T>
consteval size_t size(auto&&... args)
{
    if constexpr (requires{ T{ args... }; })
    {
        return size<T>(args..., V{});
    }
    return sizeof ...(args) - 1;
}
template<class T, class Fn> requires std::is_aggregate_v<std::decay_t<T>>
constexpr auto for_each_member(T&& t, Fn&& fn)
{
    constexpr auto num = size<std::decay_t<T>>();
    if constexpr (num == 1)
    {
        auto&& [_1] = std::forward<T>(t);
        fn(_1);
    }
    else if constexpr (num == 2)
    {
        auto&& [_1, _2] = std::forward<T>(t);
        fn(_1);fn(_2);
    }
    else if constexpr (num == 3)
    {
        auto&& [_1, _2, _3] = std::forward<T>(t);
        fn(_1);fn(_2);fn(_3);
    }
    else if constexpr (num == 4)
    {
        auto&& [_1, _2, _3, _4] = std::forward<T>(t);
        fn(_1);fn(_2);fn(_3);fn(_4);
    }
    else{}
}
int main(){
    struct X{std::string s{" "};} x;
    struct Y{double a{},b{},c{},d{};} y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto& member){
        std::cout << member << " ";
    };
    for_each_member(x, print);
    for_each_member(y,print);
}
