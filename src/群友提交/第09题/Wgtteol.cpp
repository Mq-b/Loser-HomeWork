#include<iostream>

template<class T>
struct X {
    void f()const { std::cout << "X\n"; }
};

void f() { std::cout << "全局\n"; }

template<class T>
struct Y : X<T> {
    void t()const {
        this->f();
        // 因为 Y 继承 X， 又因为 使用了 this (X<T>*)，所以是一个 实参依赖查找，所以查找的是 X<T> 实例 的f();
    }
    void t2()const {
        f();
        // 没用使用 this，也没有使用 ::， 所以是 无限定的名字查找，所以查找的是全局的 f();
    }
};

int main() {
    Y<void>y;
    y.t();
    y.t2();
}