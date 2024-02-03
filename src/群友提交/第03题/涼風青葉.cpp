#include<iostream>
#include<format>

struct Frac {
    int a, b;
};

template<>
struct std::formatter<Frac> : std::formatter<char> {
    template<typename FmtCtx>
    auto format(const Frac &frac, FmtCtx &ctx) const {
        return std::format_to(ctx.out(), "{}/{}", frac.a, frac.b);
    }
};

template<typename T, typename ...Args>
void print(const T &fmt, Args &&...args) {
    std::cout << std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
}

int main() {
    Frac f{ 1, 10 };
    print("{}", f);
}
