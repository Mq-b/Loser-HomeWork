聚合类（*Aggregate*）是一种特殊的类，具有以下的特征：

* 没有用户定义的或继承的构造函数
* 没有 `private` 或 `protected` 的非静态成员
* 没有虚函数
* 没有 `virtual`, `protected` 或 `private` 基类

聚合类可以使用聚合初始器以被初始化（Aggregate classes can be initalized by aggregate initalizer）。

```cpp
struct Point { int x, y; }; // aggregate
Point pt = {0, 1};
```

对于 `size<T>()` 函数，可以利用约束与概念，以确定聚合初始化 `T` 所需要的参数数量，从而确定成员数量。

```cpp
template <typename T>
    requires requires { T { {}, {}, {}, {} }; }
constexpr auto size_()
{
    return 4u;
}

template <typename T>
    requires requires { T { {}, {}, {} }; }
constexpr auto size_()
{
    return 3u;
}

template <typename T>
    requires requires { T { {}, {} }; }
constexpr auto size_()
{
    return 2u;
}

template <typename T>
    requires requires { T { {} }; }
constexpr auto size_()
{
    return 1u;
}

template <typename T>
    requires requires { {}; }
constexpr auto size_()
{
    return 0u;
}

template <typename T>
constexpr size_t size()
{
    static_assert(std::is_aggregate_v<T>);
    return size_<T>();
}
```

但是这假设类的成员都可以被值初始化。所以需要一个工具类，可以转换成任何内容：

```cpp
struct init
{
  template <typename T>
  operator T(); // 不需要定义
};
```

然后：

```cpp
template <typename T>
    requires requires { T { init {}, init {}, init {}, init {} }; }
constexpr auto size_()
{
    return 4u;
}

template <typename T>
    requires requires { T { init {}, init {}, init {} }; }
constexpr auto size_()
{
    return 3u;
}

template <typename T>
    requires requires { T { init {}, init {} }; }
constexpr auto size_()
{
    return 2u;
}

template <typename T>
    requires requires { T { init {} }; }
constexpr auto size_()
{
    return 1u;
}

template <typename T>
    requires requires { T {}; }
constexpr auto size_()
{
    return 0u;
}

template <typename T>
constexpr size_t size()
{
    static_assert(std::is_aggregate_v<T>);
    return size_<T>(); // highest supported number
}
```

看起来很不错，对吧？试一试：

```cpp
static_assert(size<Point>() == 2);
```

然后……

```
<source>:48:12: error: call to 'size_' is ambiguous
    return size_<T>(); // highest supported number
           ^~~~~~~~
<source>:55:15: note: in instantiation of function template specialization 'size<Point>' requested here
static_assert(size<Point>() == 2);
              ^
<source>:25:16: note: candidate function [with T = Point]
constexpr auto size_()
               ^
<source>:32:16: note: candidate function [with T = Point]
constexpr auto size_()
               ^
<source>:39:16: note: candidate function [with T = Point]
constexpr auto size_()
               ^
```

很明显，`Point{1}`、`Point{1,2}` 和 `Point{}` 都是合法的表达式，所以会报错。

为了解决这个问题，需要对 `size_` 进行的重载排序。来看这个例子：

```cpp
struct B {};
struct C : B {};

void fun(B); // (1)
void fun(C); // (2)

fun(C{});    // 选择 (2)
```

但是如果删掉 (2)，则会选择 (1)[^1]。这就是我们想要的东西。

把继承链推广到无限长度：

```cpp
template <unsigned I>
struct tag : tag<I - 1> {};

template <>
struct tag<0> {};
```

然后就得到了最终版本：

```cpp
template <typename T>
    requires requires { T { init {}, init {}, init {}, init {} }; }
constexpr auto size_(tag<4>)
{
    return 4u;
}

template <typename T>
    requires requires { T { init {}, init {}, init {} }; }
constexpr auto size_(tag<3>)
{
    return 3u;
}

template <typename T>
    requires requires { T { init {}, init {} }; }
constexpr auto size_(tag<2>)
{
    return 2u;
}

template <typename T>
    requires requires { T { init {} }; }
constexpr auto size_(tag<1>)
{
    return 1u;
}

template <typename T>
    requires requires { T {}; }
constexpr auto size_(tag<0>)
{
    return 0u;
}

template <typename T>
constexpr size_t size()
{
    static_assert(std::is_aggregate_v<T>);
    return size_<T>(tag<4> {}); // highest supported number
}
```

有了 `size<T>()`，`for_each_member` 函数就很简单了。只需要硬编码每一种可能的长度即可。

```cpp
template <typename T, typename F>
void for_each_member(T const& v, F f)
{
    static_assert(std::is_aggregate_v<T>);

    if constexpr (size<T>() == 4u) {
        const auto& [m0, m1, m2, m3] = v;
        f(m0);
        f(m1);
        f(m2);
        f(m3);
    } else if constexpr (size<T>() == 3u) {
        const auto& [m0, m1, m2] = v;
        f(m0);
        f(m1);
        f(m2);
    } else if constexpr (size<T>() == 2u) {
        const auto& [m0, m1] = v;
        f(m0);
        f(m1);
    } else if constexpr (size<T>() == 1u) {
        const auto& [m0] = v;
        f(m0);
    }
}
```

[^1]: 详见 [重载决议（Overload Resolution）](https://en.cppreference.com/w/cpp/language/overload_resolution)
