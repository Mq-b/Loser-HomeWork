//!msvc
#include <format>
#include <iostream>
#include <string_view>
#include <string>

class formator {
public:
    formator(const char* str):_inner_str(str) {}
    auto operator () (auto&&... args) {
        return std::vformat(_inner_str, std::make_format_args(std::forward<decltype(args)>(args)...));
    }
private:
    const char* _inner_str;
};


auto operator""_f(const char* str, size_t n) {
    return formator(str);
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