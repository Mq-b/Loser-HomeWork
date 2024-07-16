#include <format>
#include <iostream>
#include <numbers>

constexpr auto operator""_f(const char* str, size_t)
{
    return [=]<typename... T>(T&& ...args)
    {
        return std::vformat(str, std::make_format_args(args...));
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
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v<double>, n);
}
