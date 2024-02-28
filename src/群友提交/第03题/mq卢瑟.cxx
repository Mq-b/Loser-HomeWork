#include <iostream>
#include <format>

struct Frac {
    int a, b;
};

template<typename... Args>
void print(std::string_view format_str, Args const&... args) {
    std::cout << std::vformat(format_str, std::make_format_args(std::forward<decltype(args)>(args)...));
}

namespace std {
    template<>
    struct formatter<Frac> : formatter<string> {
        auto format(Frac const& f, format_context& ctx) const {
            return format_to(ctx.out(), "{}/{}", f.a, f.b);
        }
    };
}

int main() {
    const Frac f{ 1, 10 };
    print("{}", f); 

    return 0;
}
