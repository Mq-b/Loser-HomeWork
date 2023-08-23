#include <format>
#include <iostream>
#include <numbers>

struct myType {
    constexpr myType(std::string str_):str(str_) {}

    std::string operator()(auto &&... args) {
        return std::vformat(str, std::make_format_args(args...));
    }

    std::string str;
};
myType operator ""_f(const char* str , unsigned long long size) {
    return {str};
}
int main(){
    std::cout << "乐 :{} *\n"_f(5);
    std::cout << "乐 :{0} {0} *\n"_f(5);
    std::cout << "乐 :{:b} *\n"_f(0b01010101);
    std::cout << "{:*<10}"_f("卢瑟");

    std::cout << '\n';
    int n{};
    std::cin >> n;
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v<double>, n);
}