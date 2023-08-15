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
    auto format(Frac const & f, FormatContext& fc) const
    {
        return std::vformat_to(fc.out(),"{}/{}",std::make_format_args(f.a,f.b));
    }
};

template<typename... Args>
void print(char const * str, Args&&... args){
    std::cout << std::vformat(str,std::make_format_args(std::forward<Args>(args)...));
}

int main()
{
    Frac f{ 1,10 };

    print("{}", f);

    return 0;
}