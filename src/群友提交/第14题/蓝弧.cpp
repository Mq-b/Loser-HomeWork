#include<iostream>
#include<cstddef>
#include<cstdint>

namespace ss {
    int a = 0;
}

inline constexpr std::byte operator ""_b(unsigned long long int i)noexcept
{
    return static_cast<std::byte>(i);
}
void todo(std::byte*i)
{
    for(;*i!=0xCC_b;++i)
    {
        if(*i==0x8B_b)
        {
            *reinterpret_cast<int*>(i+*reinterpret_cast<std::int32_t*>(i+2)+6) = 100;
            break;
        }
    }
}
int main() {
    todo(reinterpret_cast<std::byte*>(main));
    std::cout << ss::a << '\n';
}
