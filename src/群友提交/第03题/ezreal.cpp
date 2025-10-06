#include <iostream>
#include <format>
#include <string_view>

struct Frac
{
    int a, b;
};

template <>
struct std::formatter<Frac> : std::formatter<char>
{
    auto format(const auto &frac, auto &ctx) const
    {
        return std::format_to(ctx.out(), "{}/{}", frac.a, frac.b);
    }
};

void print(std::string_view fmt, auto &&...args)
{
    std::cout << std::vformat(fmt, std::make_format_args(args...));
}

int main()
{
    Frac f{1, 10};
    print("{}", f); // 结果为1/10
}