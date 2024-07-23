#include<iostream>
#include<format>
#include<string.h>
#include<numbers>

struct display{
private:
    const char*  m_s;
public:
    display(const char* s) :m_s(s) {}

    template<typename...Args>
    auto operator()(Args&&...args) {
        return std::vformat(m_s, std::make_format_args(args...));
    }
};

auto operator""_f(const char* s,std::size_t n) {
    return display{s};
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
    return 0;
}
