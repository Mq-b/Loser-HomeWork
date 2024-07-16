#include <format>
#include <iostream>
#include <utility>

void print(const char* s, auto&&... vars)
{
    std::cout << std::vformat(s, std::make_format_args(vars...));
}

struct Frac
{
    int a, b;
};

template<>
struct std::formatter<Frac>: std::formatter<int>
{
    auto format(const Frac& frac, auto& context) const
    {
        return std::format_to(context.out(), "{}/{}", frac.a, frac.b);
    }
};

int main()
{
    Frac f{1, 10};
    print("{}", f);
    return 0;
}