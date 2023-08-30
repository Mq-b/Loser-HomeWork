#include <format>
#include <functional>
#include <iostream>
#include <numbers>

constexpr auto operator ""_f(const char* s, size_t l)
{
    //return [s](auto&& ...vars){ return std::vformat(s, std::make_format_args(vars...)); };
    //改用完美转发
    return [s](auto&& ...vars){ return std::vformat(s, std::make_format_args(std::forward<decltype(vars)>(vars)...)); };
}

int main()
{
    std::cout << "乐 :{} *\n"_f(5);
    std::cout << "乐 :{0} {0} *\n"_f(5);
    std::cout << "乐 :{:b} *\n"_f(0b01010101);
    std::cout << "{:*<10}"_f("卢瑟");
    std::cout << '\n';
    int n{};
    std::cin >> n;
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v<double>, n);
    return 0;
}
