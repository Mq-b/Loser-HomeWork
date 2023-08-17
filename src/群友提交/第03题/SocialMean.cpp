#include <format>
#include <iostream>
#include <string_view>

struct Frac {
  int a, b;
};
template <typename charT>
struct std::formatter<Frac, charT> : std::formatter<int, charT> {
  auto format(const Frac& frac, std::format_context& fmtctx) const {
    return std::vformat_to(fmtctx.out(), "{}/{}",
                           std::make_format_args(frac.a, frac.b));
  }
};
template <typename... Args>
void print(const char* fmt, Args&&... args) {
  std::cout << std::vformat(fmt,
                            std::make_format_args(std::forward<Args>(args)...))
            << '\n';
}

int main() {
  Frac f{1, 10};
  print("{}", f);  // 结果为1/10

  return 0;
}