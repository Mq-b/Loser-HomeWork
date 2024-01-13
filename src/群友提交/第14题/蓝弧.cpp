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
    for(;*i!=0xC3_b;++i)
    {
        if(*i==0x8B_b)
        {
            *reinterpret_cast<int*>(i+*reinterpret_cast<std::int32_t*>(i+2)+6) = 100;
            break;
        }
    }
}
void call_todo(void*)
{
    void* p = nullptr;
    void* p2 = nullptr;
    todo(reinterpret_cast<std::byte*>(*(&p+3)));
}
int main() {
    //todo(reinterpret_cast<std::byte*>(main));
    call_todo(nullptr);
    std::cout << ss::a << '\n';
}
