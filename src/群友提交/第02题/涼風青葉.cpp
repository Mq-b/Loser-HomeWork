#include<iostream>
#include<numbers>
#include<format>

struct Formatter {
    const std::string_view fmt;

    Formatter(const char *s) : fmt(s) { }

    template<typename ...T>
    std::string operator()(const T &...args) const {
        return std::vformat(fmt, std::make_format_args(args...));
    }
};

Formatter operator""_f(const char *s, std::size_t) {
    return { s };
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