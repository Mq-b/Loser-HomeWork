#include <iostream>

template <class T>
struct X
{
    void f() const { std::cout << "X\n"; }
};

void f() { std::cout << "global\n"; }

template <class T>
struct Y : X<T>
{
    void t() const { this->f(); }
    void t2() const { f(); }
};

int main()
{
    Y<void> y;
    y.t();
    y.t2();
}

// 主要看两篇zhihu和一篇文章
// 1. https://zhuanlan.zhihu.com/p/599328180 两阶段命名查找 2-Phase Name Lookup
// 2. https://zhuanlan.zhihu.com/p/560852543 c++ template 查找规则

// 简单的解释
// this->f() 这个是依赖名称，依赖了基类，而基类依赖类型T 因此查找会被延迟到Y<void> y;
// 这个时候，匹配到X::f() f() 这个不是依赖名称，直接在这一行往上找到了::f()的全局
// 如果X并不是一个模板类，Y同样继承X的话，就不涉及到模板下名称查找的规则，直接通过依赖X静态往上找，找到X::f()，会输出
// X X

// 我想问的是 题目说的文章是不是这个
// https://sg-first.gitbooks.io/cpp-template-tutorial/content/TMP_ji_chu_md.html

// 然而，Template引入的“双阶段名称查找（Two phase name lookup）”堪称是C++中最黑暗的角落 ----
// 这是LLVM的团队自己在博客上说的 —— 因此在这里，我们还是有必要去了解标准中是如何规定的。

// 其中最黑暗的角落我也赞同。并且这类代码几乎不可能在工作中出现

