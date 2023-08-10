#include <format>
#include <iostream>

struct Frac
{
    int a,b;
};

template<typename CharT>
struct std::formatter<Frac, CharT> : std::formatter<int, CharT>
{
    template<typename FormatContext>
    typename FormatContext::iterator format(const Frac& frac, FormatContext& fmt_ctx) const
    {
        typename FormatContext::iterator iter{};
        iter = std::formatter<int, CharT>::format(frac.a, fmt_ctx);
        iter = '/';
        iter = std::formatter<int, CharT>::format(frac.b, fmt_ctx);
        return iter;
    }
};

void print(const char* fmt, auto&& ...frac)
{
    std::cout<< std::vformat(fmt,std::make_format_args(std::forward<decltype(frac)>(frac)...));
}

int main()
{
    Frac f{1,10};
    print("{} {}", f, f);
    return 0;
}
