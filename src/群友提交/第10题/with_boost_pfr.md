# `10` 遍历任意类数据成员

我们给出的 `C++20` 或 `C++17` 的 `size` 的实现是有问题的，简单的说，**它没办法处理聚合类型存储数组的问题**。 在题目开头我们也说了。

我们拿 [`boost::pfr`](https://www.boost.org/doc/libs/1_82_0/doc/html/boost_pfr/tutorial.html) 的行为作为参考，我们采用 [`Boost1.82.0`](https://www.boost.org/doc/libs/1_82_0/doc/html/boost_pfr/tutorial.html) 版本。

## C++20版本

```cpp
#include <iostream>
#include<boost/pfr/functions_for.hpp>

struct init {
    template <typename T>
    operator T(); // 无定义 我们需要一个可以转换为任何类型的在以下特殊语境中使用的辅助类
};

template<typename T>
consteval size_t size(auto&&...Args) {
    if constexpr (!requires{T{ Args... }; }) {
        return sizeof...(Args) - 1;
    }
    else {
        return size<T>(Args..., init{});
    }
}
struct X { int a{ 1 }, b{ 2 }, c[2]{ 3, 4 }; };

int main(){
    std::cout << size<X>() << '\n';
    std::cout << boost::pfr::tuple_size_v<X> << '\n';//调pfr库

    std::cout << std::is_aggregate_v<X> << '\n';
}
```

[运行结果](https://godbolt.org/z/dWEK6beoK)

```text
4
4
1
```

## C++17 版本

```cpp
#include <iostream>
#include <boost/pfr/functions_for.hpp>

struct init {
    template <typename T>
    operator T(); // 无定义 我们需要一个可以转换为任何类型的在以下特殊语境中使用的辅助类
};

template <unsigned I>
struct tag :tag<I - 1> {}; //模板递归展开 继承 用来规定重载的匹配顺序 如果不这么写，匹配是无序的
template <>
struct tag<0> {};

template <typename T>//SFIANE
constexpr auto size_(tag<4>) -> decltype(T{ init{}, init{}, init{}, init{} }, 0u)
{
    return 4u;
}
template <typename T>
constexpr auto size_(tag<3>) -> decltype(T{ init{}, init{}, init{} }, 0u)
{
    return 3u;
}
template <typename T>
constexpr auto size_(tag<2>) -> decltype(T{ init{}, init{} }, 0u)
{
    return 2u;
}
template <typename T>
constexpr auto size_(tag<1>) -> decltype(T{ init{} }, 0u)
{
    return 1u;
}
template <typename T>
constexpr auto size_(tag<0>) -> decltype(T{}, 0u)
{
    return 0u;
}

template <typename T>
constexpr size_t size() {
    static_assert(std::is_aggregate_v<T>); //检测是否为聚合类型
    return size_<T>(tag<4>{}); //这里就是要求从tag<4>开始匹配，一直到tag<0>
}

struct X { int a{1}, b{2}, c[2]{3, 4}; };

int main(){
    std::cout << size<X>() << '\n';
    std::cout << boost::pfr::tuple_size_v<X> << '\n';

    std::cout << std::is_aggregate_v<X> << '\n';
}
```

[运行结果](https://godbolt.org/z/jc58bx399)

```text
4
4
1
```
