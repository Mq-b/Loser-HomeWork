#include<iostream>
#include<numbers>
#include<cstddef>
#include<array>
#include<type_traits>
#include<format>
#include<string_view>

template<typename char_t,std::size_t size>
struct S : std::array<char_t,size>
{
    consteval S(std::type_identity_t<const char_t[size]>& s)noexcept : std::array<char_t,size>(std::to_array(s)) {}
};
template<typename char_t,std::size_t size>
S(const char_t(&)[size]) -> S<char_t,size>;
template<S s>
auto operator ""_f()
{
    return [](auto&&...v)
    {
        return std::format(std::basic_string_view{s.begin(),s.end()-1},v...);//use std::format,not std::vformat
    };
}

int main() {
    std::cout << "乐 :{} *\n"_f(5);
    std::cout << "乐 :{0} {0} *\n"_f(5);
    std::cout << "乐 :{:b} *\n"_f(0b01010101);
    std::cout << "{:*<10}"_f("卢瑟");
    std::cout << '\n';
    int n{};
    std::cin >> n;
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v<double>,n);
}
