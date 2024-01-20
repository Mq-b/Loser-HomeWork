//!gcc !clang
import std;

struct Frac
{
    int a;
    int b;
};

template<>
struct std::formatter<Frac, char> : std::formatter<char>
{
    template<typename Out>
    constexpr auto format(const Frac& frac, std::basic_format_context<Out, char>& context) const
    {
        return std::format_to(context.out(), "{}/{}", frac.a, frac.b);
    }
};

void print(auto&& text, auto&&... args)
{
    std::cout << std::vformat(
        std::forward<decltype(text)>(text),
        std::make_format_args(std::forward<decltype(args)>(args)...));
}

int main()
{
    print(std::string("{}"), Frac { 1, 10 });
    return 0;
}
