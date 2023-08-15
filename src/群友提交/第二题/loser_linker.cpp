#include <format>
#include <iostream>
#include <string_view>
#include <numbers>

struct FormatterStream {
    FormatterStream(std::string_view fmt)
        :fmt_(fmt) {
    }

    template<typename... Args>
    std::string operator()(Args&&... args) {
        return std::vformat(fmt_, std::make_format_args(std::forward<Args>(args)...));
    }

    std::string fmt_;
};

FormatterStream operator ""_f(char const* str, std::size_t size) {
    return FormatterStream(str);
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