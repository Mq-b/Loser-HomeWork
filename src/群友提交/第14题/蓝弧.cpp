/*
测试能够正确运行的编译环境：
x86-64 gcc 13.2 : https://godbolt.org/z/oG53ff16n
x86-64 clang 17.0.1 : https://godbolt.org/z/E9vxf5rr5
MinGW gcc 13.2.0 : 本机 x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0 g++
x64 msvc v19.37.32824 : 本机 Release x64 /std:c++17
*/
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
    //`todo`从该地址开始搜索以`8B`开头的`mov`指令，并计算出`ss::a`的地址。
    for(;*i!=0xC3_b;++i)//函数以`ret`指令（机器码为`C3`）结束，这是失败路径防止意外修改的代码。
    {
        if(*i==0x8B_b)
        {
            //（`ss::a`的引用即`[i+[i+2]+6]`，其中`i`是该条`mov`指令的开始位置，`[i+2]`是`mov`指令中存储的偏移地址，`6`是该条指令的长度）
            *reinterpret_cast<int*>(i+*reinterpret_cast<std::int32_t*>(i+2)+6) = 100;
            break;
        }
    }
}
int main() {
    todo(reinterpret_cast<std::byte*>(main));
    //下面这段代码读取了`ss::a`，我就希望从这段代码入手获取`ss::a`的地址，经过测试发现，读取`ss::a`的通常是一条以`8B`开头的`mov`指令，于是就有了`todo`函数；
    std::cout << ss::a << '\n';
}
