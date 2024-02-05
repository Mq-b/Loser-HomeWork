#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
using namespace std;
class Frac
{
public:
    int a, b;
};

template<> struct std::formatter<Frac> : std::formatter<int>
{
    auto format(const Frac &frac, auto &ctx) const
    {
        return std::format_to(ctx.out(), "{}/{}", frac.a, frac.b);
    }
};

void print(const char *str, auto &&...args)
{
    std::cout << std::vformat(str, std::make_format_args(std::forward<decltype(args)>(args)...));
}

int main()
{
    Frac f{1, 10};
    print("{}", f);
    return 0;
} // 结果为1/10