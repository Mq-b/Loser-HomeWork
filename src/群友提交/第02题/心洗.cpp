import std;

#define USE_LAMBDA
#ifdef USE_LAMBDA
struct format
{
    format(const char* s) :m_const_ch(s)
    {

    }

    template<typename... Args>
    auto operator()(Args... args)
    {
        return std::vformat(m_const_ch, std::make_format_args(args...));
    }

private:
    std::string_view m_const_ch;
};

auto operator ""_f(const char* text, size_t)
{
    return format(text);
}

#else // another implementation
auto operator ""_f(const char* text, size_t)
{
    return [text](auto&&... args) {
        return std::vformat(text, std::make_format_args(std::forward<decltype(args)>(args)...));
        };
}
#endif

int main()
{
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