#include <format>
#include <iostream>

struct Frac {
    int a, b;
};

template <typename CharT>
struct std::formatter<Frac, CharT> : std::formatter<int, CharT> {
    template <typename FormatContext>
    auto format(const Frac& frac, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(), "{}/{}", frac.a, frac.b);
    }
};

constexpr void print(const char* fmt, auto&&... args)
{
    std::fputs(std::vformat(fmt, std::make_format_args(std::forward<decltype(args)>(args)...)).c_str(), stdout);
}

int main()
{
    Frac f{1, 10};
    print("{} {}", f, f);
    return 0;
}
