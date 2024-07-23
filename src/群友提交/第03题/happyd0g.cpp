#include <format>
#include <iostream>

class Frac {
public:
    int key,value;
};

template<>
struct std::formatter<Frac> : std::formatter<int> {
    auto format(const Frac& frac, auto& ctx) const {
        return std::format_to(ctx.out(), "key={}, value={}", frac.key, frac.value);
    }
};

void print(const char* str, auto&& ... args) {
    std::cout << std::vformat(str, std::make_format_args(args...));
} 

int main()
{
    Frac f{ 1,10 };

    print("{} {}", f, 22);

    return 0;
}