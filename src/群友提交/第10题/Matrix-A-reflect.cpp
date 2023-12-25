#include<iostream>
#include<cstdint>
#include<vector>
#include<mutex>
#include<experimental/reflect>

namespace reflect = std::experimental::reflect;

template<typename T>
    requires std::is_aggregate_v<T>
constexpr std::size_t size() {
    if constexpr (std::is_array_v<T>) {
        return std::extent_v<T>;
    }
    else {
        using Reflected_t = reflexpr(T);
        using data_members = reflect::get_data_members_t<Reflected_t>;
        return reflect::get_size_v<data_members>;
    }
}


template<typename T, typename Func>
    requires std::is_aggregate_v<T>
void for_each_member(const T& t, Func func) {
    constexpr auto T_size = size<T>();
    if constexpr (std::is_array_v<T>) {
        for (const auto& i : t) {
            func(i);
        }
    }
    else {
        using Reflected_t = reflexpr(T);
        using data_members = reflect::get_data_members_t<Reflected_t>;
        [&] <size_t... ints>(std::index_sequence<ints...>) {
            std::tuple args{ reflect::get_pointer_v<reflect::get_element_t<ints, data_members>>... };
            auto application = [&](auto&&... args) { (func(t.*args), ...); };
            std::apply(application, args);
        }(std::make_index_sequence<T_size>{});
    }
}

int main() {
    std::string ts{ "123" };
    struct X { const std::string s; } x{ ts };
    struct Y { double a[2]{ 3., 2. }, b{}, c{}, d{}; } y;
    Y ys[10];
    struct Z { std::mutex m; } z;
    std::cout << size<X>() << std::endl;
    std::cout << size<Y>() << std::endl;
    std::cout << size<decltype(ys)>() << std::endl;
    std::cout << size<Z>() << std::endl;
    auto print = [](const auto& member) {
        if constexpr (!requires { std::cout << member; }) {
            std::cout << "[" << &member << "] ";
        }
        else {
            std::cout << member << ' ';
        }
        };
    for_each_member(x, print);
    std::cout << std::endl;
    for_each_member(y, print);
    std::cout << std::endl;
    for_each_member(ys, print);
    std::cout << std::endl;
    for_each_member(z, print);
    std::cout << std::endl;
}
