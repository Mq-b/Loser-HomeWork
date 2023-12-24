#include<iostream>

// 宏，把 a 变成带有初始化的变量
#define a a = 100, b
namespace ss1 {
    int a = 0;
}
#undef a
// 宏，拼接出来一个变量名
namespace ss2 {
    int a = 0;
}
// 宏，输出前赋值
namespace ss3 {
    int a = 0;
}
// 宏，修改为无名命名空间
#define ss4
namespace ss4 {
    int a = 0;
}
// 宏，修改a变量为结构体
#define int struct A{  \
    A(int) : impl{100} {}  \
    friend std::ostream& operator<<(std::ostream& out, const A& a) { \
        return out<<a.impl; \
    } \
    int impl; \
}
namespace ss5 {
    int a = 0;
}
#undef int
// 利用同命名空间变量初始化修改
namespace ss6 {
    int a = 0;
}
namespace ss6 {
    int _{ a = 100 };
}
// 直接内联
inline
namespace ss7 {
    int a_ = 0; // 和 ss4 里的 a 重名了
}
// 利用名字查找规则
namespace ss8 {
    int a = 0;
}
namespace ss8 {
    extern int b;
}
int ss8::b = a = 100;

int main() {
    {
        std::cout << ss1::a << '\n';
    }
    {
#define xnv(x1, x2, x3, x4) x1##x2##x3::x4
        xnv(s, s, 2, a) = 100;
        std::cout << ss2::a << '\n';
    }
    {
#define ss3 (ss3
#define a a = 100)
        std::cout << ss3::a << '\n';
#undef a
#undef ss3
    }
    {
        a = 100;
        std::cout << ss4::a << '\n';
    }
    {
        std::cout << ss6::a << '\n';
    }
    {
        std::cout << ss5::a << '\n';
    }
    {
        a_ = 100;
        std::cout << ss7::a_ << '\n';
    }
    {
        std::cout << ss8::a << '\n';
    }
}
