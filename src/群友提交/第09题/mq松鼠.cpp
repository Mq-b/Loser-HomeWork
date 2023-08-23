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
        // 因为当前类是待决类型 所以this 是待决名 所以this->f是类型待决表达式
        // 对于f的名字查找会延后到进行实例化的时候
    }
    void t2()const {
        f();
        // f是非待决名 基类 X<T>取决于模板形参T，所以无限定名字查找不会检查它的作用域
        // 所以只能找到全局的f
    }
};

int main() {
    Y<void>y;// 实例化后进行非限定名字查找
    y.t();
    y.t2();
}