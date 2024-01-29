#include<iostream>
#include<format>

struct Frac {
    int a, b;
};

template<>
struct std::formatter<Frac> : std::formatter<char> {
    auto format(const Frac &arg, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}/{}", arg.a, arg.b);
    }
};

template<typename T, typename ...U>
void print(T fmt, U ...args) {
    std::cout << std::vformat(fmt, std::make_format_args(args...));
}

int main() {
    Frac f{ 1, 10 };
    print("{}", f);
}