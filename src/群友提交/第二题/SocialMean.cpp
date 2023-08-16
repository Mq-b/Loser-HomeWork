#include <format>
#include <functional>
#include <iostream>
#include <numbers>
#include <string>
#include <string_view>

auto operator""_f(const char* str, size_t len) {
  std::string_view fmt_str = std::string_view(str, len);
  return [fmt_str](auto&&... args) {
    return std::vformat(fmt_str, std::make_format_args(args...));
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