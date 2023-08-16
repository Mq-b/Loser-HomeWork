#include<iostream>

template<class T>
struct X {
    void f()const { std::cout << "X\n"; }
};

void f() { std::cout << "全局\n"; }

// 普通类的非静态成员函数会将函数体中的非类型非静态成员的标识表达式e变换(*this).e 而类模板不会变换
template<class T>
struct Y : X<T> {
    // 因为T是待决类型 所以X<T> 是待决类型  但是X<T>不是当前实例化 -> 所以X<T>是待决基类
    void t()const {
        this->f();
        // 这是一个类成员访问表达式
        // this的类型是Y* 等价于Y<T>* 所以是当前实例化
        // 在this的当前实例化中找到了从X<T>继承来的f名字
    }
    void t2()const {
        f();
        // 基类 X<T>取决于某个模板形参，所以无限定名字查找不会检查它的作用域
        // 没有在模板定义语境中进行向类成员访问语法的变换 于是找不到继承来的f 只能找到全局的f
    }
};

int main() {
    Y<void>y;// 实例化后进行非限定名字查找
    y.t();
    y.t2();
}