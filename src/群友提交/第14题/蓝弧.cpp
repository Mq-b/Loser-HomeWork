/*
测试能够正确运行的编译环境：
x86-64 gcc 13.2 : https://godbolt.org/z/TdjE64bY4
x86-64 clang 17.0.1 : https://godbolt.org/z/8jTsWTMqh
MinGW gcc 13.2.0 : 本机 x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev0 g++
x64 msvc v19.37.32824 : 本机 Release x64 /std:c++17 /sdl- 注意需要关闭SDL检查
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
void call_todo(void*)
{
    //参数`void*`、局部变量`void* p2`是用来对齐不同编译器栈中内存布局不同
    void* p = nullptr;
    void* p2 = nullptr;
    todo(reinterpret_cast<std::byte*>(*(&p+3)));//将该函数的返回地址（调用该函数时call指令压入栈中的地址）传递给todo
}
int main() {
    //todo(reinterpret_cast<std::byte*>(main));
    call_todo(nullptr);//使用`call_todo`是为了避免[CWG2811](https://cplusplus.github.io/CWG/issues/2811.html)中不得使用`main`函数的规定。
    //下面这段代码读取了`ss::a`，我就希望从这段代码入手获取`ss::a`的地址，经过测试发现，读取`ss::a`的通常是一条以`8B`开头的`mov`指令，于是就有了`todo`函数；
    std::cout << ss::a << '\n';
}
