+++
title = "重载赋值运算符是否应检查自赋值"
type = "docs"
draft = false
+++

在初学 C++ 运算符重载的时候，重载 `=` 运算符，基本都被教导过要**检测自赋值的情况**。

```cpp
class Foo {
    std::string s;
    int i;
public:
    Foo& operator=(const Foo& a){
        if (this == &a) return *this;    // OK，但是多余
        s = a.s;
        i = a.i;
        return *this;
    }
};
```

但是，你真的**必须要检测**吗？显然不是，教条主义不可取。

- **需要保证自赋值安全，不代表必须要像那样进行检测。**

## 正文

在讨论“是否要检测自赋值”之前，我们应该先明确另一个问题：

- 自赋值很常见吗？正常逻辑会遇到吗？

**自赋值并不算很常见但是需要处理，正常逻辑的确会遇到**。如下：

```cpp
struct Test{
    int v{};
    Test& operator=(const Test& test) {
        if (this == std::addressof(test)){ // 多余，纯属为了展示
            std::puts("自赋值");
            return *this;
        }
        v = test.v;
        return *this;
    }
    bool operator <(const Test& test)const noexcept{
        return v < test.v;
    }
};

int main(){
    Test t{ 1 };
    Test t2{ 2 };
    t2 = std::max(t, t2);    // 合理
    t = t;                   // 不合理
}
```

我们可以看看标准库是如何处理的，这里以 [**msvc STL**](https://github.com/microsoft/STL) 的 std::vector、std::string、std::shared_ptr 的 operator= 为例。

```cpp
_CONSTEXPR20 vector& operator=(const vector& _Right) {
    if (this == _STD addressof(_Right)) {
        return *this;
    }

    auto& _Al       = _Getal();
    auto& _Right_al = _Right._Getal();
    if constexpr (_Choose_pocca_v<_Alty>) {
        if (_Al != _Right_al) {
            _Tidy();
            _Mypair._Myval2._Reload_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Al), _GET_PROXY_ALLOCATOR(_Alty, _Right_al));
        }
    }

    _Pocca(_Al, _Right_al);
    auto& _Right_data = _Right._Mypair._Myval2;
    _Assign_counted_range(_Right_data._Myfirst, static_cast<size_type>(_Right_data._Mylast - _Right_data._Myfirst));

    return *this;
}
```

```cpp
_CONSTEXPR20 basic_string& operator=(const basic_string& _Right) {
    if (this == _STD addressof(_Right)) {
        return *this;
    }

    auto& _Al             = _Getal();
    const auto& _Right_al = _Right._Getal();
    if constexpr (_Choose_pocca_v<_Alty>) {
        if (_Al != _Right_al) {
            auto&& _Alproxy       = _GET_PROXY_ALLOCATOR(_Alty, _Al);
            auto&& _Right_alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Right_al);
            _Container_proxy_ptr<_Alty> _New_proxy(_Right_alproxy, _Leave_proxy_unbound{}); // throws

            if (_Right._Mypair._Myval2._Large_string_engaged()) {
                const auto _New_size     = _Right._Mypair._Myval2._Mysize;
                const auto _New_capacity = _Calculate_growth(_New_size, 0, _Right.max_size());
                auto _Right_al_non_const = _Right_al;
                const auto _New_ptr      = _Right_al_non_const.allocate(_New_capacity + 1); // throws

                _Start_element_lifetimes(_Unfancy(_New_ptr), _New_size + 1);

                _Traits::copy(_Unfancy(_New_ptr), _Unfancy(_Right._Mypair._Myval2._Bx._Ptr), _New_size + 1);
                _Tidy_deallocate();
                _Mypair._Myval2._Bx._Ptr = _New_ptr;
                _Mypair._Myval2._Mysize  = _New_size;
                _Mypair._Myval2._Myres   = _New_capacity;
            } else {
                _Copy_assign_val_from_small(_Right);
            }

            _Pocca(_Al, _Right_al);
            _New_proxy._Bind(_Alproxy, _STD addressof(_Mypair._Myval2));
            return *this;
        }
    }

    _Pocca(_Al, _Right_al);
    assign(_Right._Mypair._Myval2._Myptr(), _Right._Mypair._Myval2._Mysize);
    return *this;
}
```

```cpp
shared_ptr& operator=(const shared_ptr& _Right) noexcept {
    shared_ptr(_Right).swap(*this);
    return *this;
}
```

如你所见，std::vector，std::string 均检测自赋值，std::shared_ptr 则没有，甚至是**强异常安全**的，声明为 noexcept。

std::shared_ptr 的很好解释，[copy-and-swap](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom) 理所应当这样，不需要检测。

我们举个简单的示例：

```cpp
class dumb_array{
public:
    friend void swap(dumb_array& first, dumb_array& second){
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;

        swap(first.mSize, second.mSize);
        swap(first.mArray, second.mArray);
    }
    dumb_array& operator=(dumb_array other){
        swap(*this, other);

        return *this;
    }
};
```

这理所应当，不是吗？`operator=` 不需要检测任何东西。

或者我们再聊细一点，std::shared_ptr 的 swap 的实现是这样的：

```cpp
void swap(shared_ptr& _Other) noexcept {
    this->_Swap(_Other);
}
```

只是转发了参数，还要调用内部的接口 `_Swap`

```cpp
void _Swap(_Ptr_base& _Right) noexcept { // swap pointers
    _STD swap(_Ptr, _Right._Ptr);
    _STD swap(_Rep, _Right._Rep);
}
```

成员的声明：

```cpp
element_type* _Ptr{nullptr};
_Ref_count_base* _Rep{nullptr};
```

std::shared_ptr 本身只包含两个对象：指向控制块对象的指针 `_Rep` 和一个指向其管理的资源的指针 `_Ptr`。

最终无非是交换指针罢了，即使自赋值，也不会带来什么问题和开销，当然也是能保证**自赋值安全**。

我们回到最初的示例：

```cpp
class Foo {
    std::string s;
    int i;
public:
    Foo& operator=(const Foo& a){
        if (this == &a) return *this;    // OK，但是多余
        s = a.s;
        i = a.i;
        return *this;
    }
};
```

那么你觉得它需要检测自赋值吗？我可以很明确的告诉你，**不需要**。

- ***简单的赋值操作不应该包含 `if (this == &a) return *this`***。

这里不单单是不需要 `if (this == &a) return *this`，甚至这个显式的用户定义的赋值运算符都不需要，它还会影响移动语义的使用。

```cpp
class Foo {
    std::string s;
    int i;
};
```

- 从处理自赋值的成员生成的默认赋值可以正确的处理自赋值。

我们前面也看了 std::string 的 operator= 它显然是处理了的。再说 int，赋值也无所谓。

## 总结

我们回到开头的最后一句话：

- **需要保证自赋值安全，不代表必须要像那样进行检测。**

前面也已经聊了，两种情况：

- copy-and-swap
- 简单的赋值操作

都不需要进行自赋值检测 `if (this == &a) return *this`。这里的“简单赋值操作”，可以指代的比较宽泛：比如全部都是平凡的，可以正常进行拷贝，能保证自赋值安全的对象；又或者像前面提的 `Foo` 那种，对象保有的成员就是处理了自赋值的，那生成的默认赋值运算符自然也没问题了。

以及，需要检查自赋值的情况 `if (this == &a) return *this` 其实也说了，像 std::vector、std::string，自赋值可能导致更高的开销，并且产生问题，**它们都是比较复杂的类型**。

不过我们还可以举一个简单的例子：

```cpp
struct X{
    int* ptr;
    X(int* p) :ptr(p) {}
    ~X() { delete ptr; }
    X& operator=(const X& x){  // 没有保证自赋值安全
        delete ptr;
        ptr = new int(*x.ptr);
        return *this;
    }
};

int main(){
    X x{ new int(1) };
    x = x;                    // 有问题
}
```

我们可以通过简单的添加一个 `if (this == &a) return *this` 让它**变成自赋值安全**，当然了，也可以选择使用像 std::shared_ptr 一样的方式。

一般情况下，不推荐简单的自定义类型或者**天生已经自赋值安全的类型**使用 `if (this == &a) return *this` ，这是多余的，虽然不一定很重要，但是的确会带来额外的开销。**不过保证自赋值安全，依然是基本要求**。
