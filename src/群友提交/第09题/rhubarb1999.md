# 名字查找的问题

```c++
#include<iostream>

template<class T>
struct X {
    void f()const { std::cout << "X\n"; }//声明2，类内可见
};

void f() { std::cout << "全局\n"; }//声明1，全局可见

template<class T>
struct Y : X<T> {
    void t()const {
        this->f();//绑定X::f().     解释2
    }
    void t2()const {
        f();//绑定::f().        解释1
    }
};

int main() {
    Y<void>y;
    y.t();
    y.t2();
}
```

> 解释 1
>> 对于在模板的定义中所使用的非待决名，当检查该模板的定义时将进行无限定的名字查找。因为 X::f() 的作用域不在 Y 中，所以Y中的 f() 查找到 ::f() ，并这个时候绑定。
>
> 解释 2
>> 在类模板中， this 是一个待决表达式，而且显式的 this-> 可以用于强行使另一表达式变为待决的。所以 this->f() 需要推迟到知道X类型的模板实参才会进行查找。然后类的成员在对其类或其派生的类的指针类型的表达式运用 -> 运算符之后， X::f() 的作用域在 Y 中，查找到 X::f() ，所以 this->f() 绑定 X::f() 。
