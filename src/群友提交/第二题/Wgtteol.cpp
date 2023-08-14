#include <iostream>
#include <format>
#include <numbers>
/*
struct X {
    size_t operator() (size_t n) {
        return n;
    }
};

X operator ""_f(const char* str_c, size_t n)
{
    return {};
}
*/

auto operator ""_f(const char* str_c, size_t n)
{
    return [=] (auto&& ...elems)
    {
        return std::vformat(str_c, std::make_format_args(elems...));
    };
}

int main() {
    //std::cerr << "12345xyz"_f(13) << std::endl;
    std::cout << "乐 :{} *\n"_f(5);
    std::cout << "乐 :{0} {0} *\n"_f(5);
    std::cout << "乐 :{:b} *\n"_f(0b01010101);
    std::cout << "{:*<10}"_f("卢瑟");
    std::cout << '\n';
    int n{};
    std::cin >> n;
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v<double>, n);
}
