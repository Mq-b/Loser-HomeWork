#include <format>
#include <iostream>

auto operator""_f(const char *fmt, size_t len)
{
    return [fmt](auto&& ...elems)
    {
        return std::vformat(fmt, std::make_format_args(elems...));
    };
}

int main()
{
    std::cout << "乐 :{} *\n"_f(5);
    std::cout << "乐 :{0}{0} *\n"_f(5);
    std::cout <<"乐 :{:b} *\n"_f(0b01010101);
    std::cout <<"{:*<10}"_f("卢瑟");
    std::cout << '\n';
    int n{};
    std::cin >> n;
    std::cout << "π: {:.{}f}\n"_f(std::numbers::pi_v<double>, n);
}
