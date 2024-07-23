#include <iostream>
#include <format>

struct Frac {
    int a, b;
};

template<typename... Args>
void print(std::string_view format_str, Args const&... args) {
    std::cout << std::vformat(format_str, std::make_format_args(args...));
}

template<>
struct std::formatter<Frac> : std::formatter<string> {
    auto format(Frac const& f, auto& ctx) const {
        return std::format_to(ctx.out(), "{}/{}", f.a, f.b);
    }
};

int main() {
    const Frac f{ 1, 10 };
    print("{}", f); 

    return 0;
}
