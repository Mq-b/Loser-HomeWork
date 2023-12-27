代码

``` c++
#include<iostream>

template<class T>
struct X {
    void f() const { std::cout << "X\n"; }
};

void f() { std::cout << "全局\n"; }

template<class T>
struct Y : X<T> {
    void t() const {
        this->f();
    }

    void t2() const {
        f();
    }
};

int main() {
    Y<void> y;

    y.t();

    y.t2();
}
```

输出

    X
    全局

原因

# t

    f是待决名，原因如下：Y类型取决与X<T>，  
    T是模板形参，T是待决类型，所以X<T>和Y类型都是待决类型，  
    所以this表达式是待决表达式，所以this->f()的f是待决名，而对于在模板定义中所使用的待决名，它的查找会推迟到它的实例化点,此时查找会检查基类的作用域，所以f绑定到基类中的f。

# t2

    f是非待决名，在定义处查找，因为是非待决名，无论哪种情况，如果类的某个基类取决于某个模板形参，那么无限定名字查找不会检查它的作用域（在定义点和实例化点都不会）。所以不在待决基类的作用域中查找，绑定到全局作用域的f。
    非待决名在模板定义点查找并绑定。即使在模板实例化点有更好的匹配，也保持此绑定，所以最终绑定到全局的f。
