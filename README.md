<div id="img" align=center>

[![视频教程](https://img.shields.io/badge/%E8%A7%86%E9%A2%91%E6%95%99%E7%A8%8B-bilibili-cyan)](https://www.bilibili.com/video/BV1Zj411r7eP)
[![QQ 群](https://img.shields.io/badge/QQ%E7%BE%A4-%E5%8D%A2%E7%91%9F%E5%B8%9D%E5%9B%BD-blue)](https://qm.qq.com/cgi-bin/qm/qr?k=X-ouAYdQzPDQGUR7R-vECHDpXb7Uihdm&jump_from=webapi&authKey=5XYoNIfb913mo5Ff3P1nOhVy1pJgCM4Q6wAykQ+rpiDQSRu+tCXMN6yGOkjxIIrl)
[![知乎](https://img.shields.io/badge/%E7%9F%A5%E4%B9%8E-mq%E7%99%BD-yello)](https://www.zhihu.com/people/o4ze4r)
[![youtube](https://img.shields.io/badge/video-YouTube-red)](https://www.youtube.com/channel/UCey35Do4RGewqr-6EiaCJrg)

[![modern cpp](https://img.shields.io/badge/code-Modern%20C++-blue)](https://learn.microsoft.com/zh-cn/cpp/cpp/welcome-back-to-cpp-modern-cpp)
[![cpp2b](https://img.shields.io/badge/std-C++20/23/26-yello)](https://zh.cppreference.com/w/cpp/compiler_support)
[![gcc](https://img.shields.io/badge/compiler-GCC13-white)](https://gcc.gnu.org/onlinedocs/13.2.0/)
[![msvc](https://img.shields.io/badge/compiler-MSVC19.37-white)](https://learn.microsoft.com/zh-cn/cpp/cpp)
[![clang](https://img.shields.io/badge/compiler-clang17-white)](https://releases.llvm.org/17.0.1/tools/clang/docs/)

</div>
<div id="img" align=center>
    <a herf="https://zh.cppreference.com/w/cpp"><img src="image/cpp.svg" width=64px alt="cpp"/></a>
</div>

<details>
    <summary style="font-size:150%">目录</summary>

- [前言](#前言)
- [`01` 实现管道运算符](#01-实现管道运算符)
  + [运行结果](#运行结果)
  + [群友提交](#群友提交)
  + [标准答案](#标准答案)
  + [解析](#解析)
- [`02` 实现自定义字面量 `_f`](#02-实现自定义字面量-_f)
  + [运行结果](#运行结果-1)
  + [群友提交](#群友提交-1)
  + [标准答案](#标准答案-1)
  + [解析](#解析-1)
- [`03` 实现 `print` 以及特化 `std::formatter`](#03-实现-print-以及特化-stdformatter)
  + [运行结果](#运行结果-2)
  + [群友提交](#群友提交-2)
  + [标准答案](#标准答案-2)
  + [解析](#解析-2)
- [`04` 给定类模板修改，让其对每一个不同类型实例化有不同 ID](#04-给定类模板修改让其对每一个不同类型实例化有不同-id)
  + [运行结果](#运行结果-3)
  + [群友提交](#群友提交-3)
  + [标准答案](#标准答案-3)
- [`05` 实现 `scope_guard` 类型](#05-实现-scope_guard-类型)
  + [运行结果](#运行结果-4)
  + [群友提交](#群友提交-4)
  + [标准答案](#标准答案-4)
- [`06` 解释 `std::atomic` 初始化](#06-解释-stdatomic-初始化)
  + [群友提交](#群友提交-5)
  + [标准答案](#标准答案-5)
- [`07` `throw new MyException`](#07-throw-new-myexception)
  + [运行结果](#运行结果-5)
  + [群友提交](#群友提交-6)
  + [标准答案](#标准答案-6)
- [`08` 定义`array`推导指引](#08-定义array推导指引)
  + [运行结果](#运行结果-6)
  + [群友提交](#群友提交-7)
  + [标准答案](#标准答案-7)
- [`09` 名字查找的问题](#09-名字查找的问题)
  + [运行结果](#运行结果-7)
  + [群友提交](#群友提交-8)
  + [标准答案](#标准答案-8)
- [`10` 遍历任意类数据成员](#10-遍历任意类数据成员)
  + [运行结果](#运行结果-8)
  + [群友提交](#群友提交-9)
  + [标准答案](#标准答案-9)
    - [`C++17` 写法](#c17-写法)
    - [`C++20` 写法](#c20-写法)
  + [补充说明](#补充说明)
- [`11` `emplace_back()` 的问题](#11-emplace_back-的问题)
  + [群友提交](#群友提交-10)
  + [标准答案](#标准答案-10)
- [`12` 实现`make_vector()`](#12-实现make_vector)
  + [运行结果](#运行结果-9)
  + [群友提交](#群友提交-11)
  + [标准答案](#标准答案-11)
    - [运行结果](#运行结果-10)
- [`13` 关于 `return std::move`](#13-关于-return-stdmove)
  + [群友提交](#群友提交-12)
  + [标准答案](#标准答案-12)
- [`14` 以特殊方法修改命名空间中声明的对象](#14-以特殊方法修改命名空间中声明的对象)
  + [运行结果](#运行结果-11)
  + [群友提交](#群友提交-13)
  + [标准答案](#标准答案-13)
    - [利用符号来解决](#利用符号来解决)
    - [直接修改内存](#直接修改内存)
    - [利用名字查找规则](#利用名字查找规则)
- [`15` 表达式模板](#15-表达式模板)

</details>

<!-- []: https://zh.cppreference.com/w/ -->

[Mq-b/Cookbook/7.6]: https://github.com/Mq-b/Cpp20-STL-Cookbook-src#76%E4%BD%BF%E7%94%A8%E6%A0%BC%E5%BC%8F%E5%BA%93%E6%A0%BC%E5%BC%8F%E5%8C%96%E6%96%87%E6%9C%AC
[cpp/language/range-for]: https://zh.cppreference.com/w/cpp/language/range-for
[cpp/utility/functional/function]: https://zh.cppreference.com/w/cpp/utility/functional/function
[cpp/utility/tuple]: https://zh.cppreference.com/w/cpp/utility/tuple
[cpp/ranges/range]: https://zh.cppreference.com/w/cpp/ranges/range
[cpp/language/user_literal]: https://zh.cppreference.com/w/cpp/language/user_literal

---

## 前言

**卢瑟们的作业展示。**

[提交 PR](提交pr教程.md) 不应当更改当前 `README`，请将作业提交到 `src\群友提交` 中，比如你要提交第一个作业：

你应当在 `src\群友提交\第01题` 中创建一个自己的 `.md` 或 `.cpp` 文件，**文件名以自己交流群 ID 命名（或 GitHub 用户名都可，方便找到本人即可）**。

答题的**一般要求**如下（题目额外要求也自行注意看）：

1. 不更改 `main` 函数，不得使其不运行（意思别捞偏门）。
2. 自行添加代码，在满足第一点的要求下，要能成功编译运行并与 **给出运行结果一致**。

## `01` 实现管道运算符

日期：**`2023/7/21`** 出题人：**`mq白`**

给出代码：

```cpp
int main(){
    std::vector v{1, 2, 3};
    std::function f {[](const int& i) {std::cout << i << ' '; } };
    auto f2 = [](int& i) {i *= i; };
    v | f2 | f;
}
```

### 运行结果

```text
1 4 9
```

- 难度：**★☆☆☆☆**

### [群友提交](src/群友提交/第01题)

答题者：[**`andyli`**](src/群友提交/第01题/andyli.cpp)

```cpp
#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

template <typename R, typename F>
auto operator|(R&& r, F&& f) {
    for (auto&& x: r)
        f(x);
    return r;
}
int main() {
    std::vector v{1, 2, 3};
    std::function f{[](const int& i) { std::cout << i << ' '; }};
    auto f2 = [](int& i) { i *= i; };
    v | f2 | f;
}
```

> 很常规，没啥问题。

答题者：[**`mq松鼠`**](src/群友提交/第01题/mq松鼠.cpp)

```cpp
#include <iostream>
#include <vector>
#include <functional>

auto operator | (std::vector<int>&& v,std::function<void(const int&)> f){
    for(auto&i:v){
        f(i);
    }
    return v;
}
auto operator | (std::vector<int>& v,std::function<void(int&)> f){
    for(auto&i:v){
        f(i);
    }
    return v;
}
int main(){
    std::vector v{1, 2, 3};
    std::function f {[](const int& i) {std::cout << i << '\n'; } };
    auto f2 = [](int& i) {i *= i; };
    v | f2 | f;
}
```

> 评价：闲的没事多写个重载，裱起来。

### 标准答案

```cpp
template<typename U, typename F>
    requires std::regular_invocable<F, U&>//可加可不加，不会就不加
std::vector<U>& operator|(std::vector<U>& v1, F f) {
    for (auto& i : v1) {
        f(i);
    }
    return v1;
}
```

**不使用模板**：

```cpp
std::vector<int>& operator|(std::vector<int>& v1, const std::function<void(int&)>& f) {
    for (auto& i : v1) {
        f(i);
    }
    return v1;
}
```

不使用范围 `for`，使用 C++20 简写函数模板：

```cpp
std::vector<int>& operator|(auto& v1, const auto& f) {
    std::ranges::for_each(v1, f);
    return v1;
}
```

**各种其他答案的范式无非就是这些改来改去了，没必要再写。**

### 解析

很明显我们需要重载管道运算符 |，根据我们的调用形式 `v | f2 | f`, 这种链式的调用，以及根据给出运行结果，我们可以知道，重载函数应当返回 v 的引用，并且 v 会被修改。
`v | f2` 调用 `operator |`，operator | 中使用 f2 遍历了 v 中的每一个元素，然后返回 v 的引用，再 | f。

```c++
template<typename U, typename F>
requires std::regular_invocable<F, U&> //我们可以认为对模板形参U，F满足std::regular_invocable的约束
```

如果没接触过约束表达式，没关系，下面将简要的介绍。

requires 表达式如同一个返回 bool 的函数，而 U 和 F 作为类型填入 std::regular_invocable 的实参列表里，只要作为类型的 U、F 满足该表达式则返回 true；不满足则返回 false，称为“不满足约束”。不满足约束的类型自然不会执行后续的代码。

而 [std::regular_invocable](https://zh.cppreference.com/w/cpp/concepts/invocable) 我们可以简单看成对类型 U 的每一个值，我们是否可以调用函数 F，即调用 `std::invoke` 。

这就相当于我们在编译期对运行期做了想象，想象是否可以对 U 在运行期执行 F。如果可以那满足约束。

而函数主体则极为简单

```c++
std::vector<U>& operator|(std::vector<U>& v1, const F f) {
    for (auto& i : v1) {
        f(i);
    }
    return v1;
}
```

其中[范围表达式][cpp/language/range-for] `for (auto& i : v1)`，如同 `for(auto i=v.begin();i!=v.end();++i){f(*i)}`：我们对 *vector*（范围）中的每一个元素应用一次 **f** 函数。返回时照常返回 v1。

如若不使用模板，则我们的形参列表得用 [std::function][cpp/utility/functional/function] 来接住我们使用的函数：<br/>
对范围中的每个成员应用 **f** 不需要返回值且需要对范围中的元素进行修改，所以第二个形参为 `std::function<void(int&)>`。并且我们不需要对传进来的函数 **f** 进行修改与拷贝，所以加上 **const** 限定是个好习惯。

同样的我们可以不使用范围 for 而是更简单的 `std::ranges::for_each(v1, f);` 即同上一样对范围 v1内的每个元素，应用一次函数 **f**。

对于使用模板的形式，我们可以使用 C++20 的简写函数模板；简而言之，在函数形参列表中 auto 占位符会为模板形参列表追加一个虚设的模板形参。最开始的模板形式可以写成

```c++
std::vector<int>& operator|(auto& v1, const auto& f) 
```

它和原形式相同。

---

## `02` 实现自定义字面量 `_f`

日期：**`2023/7/22`** 出题人：**`mq白`**

给出代码：

```cpp
int main(){
    std::cout << "乐 :{} *\n"_f(5);
    std::cout << "乐 :{0} {0} *\n"_f(5);
    std::cout << "乐 :{:b} *\n"_f(0b01010101);
    std::cout << "{:*<10}"_f("卢瑟");
    std::cout << '\n';
    int n{};
    std::cin >> n;
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v<double>, n);
}

```

### 运行结果

```text
乐 :5 *
乐 :5 5 *
乐 :1010101 *
卢瑟******
6
π：3.141593
```

- 难度：**★★☆☆☆**

`6` 为输入，决定 $\pi$ 的小数点后的位数，可自行输入更大或更小数字。
提示：C++11 用户定义字面量、C++20 format 库。
难度：**★★☆☆☆**

### [群友提交](src/群友提交/第02题)

答题者：[**`andyli`**](src/群友提交/第02题/andyli.cpp)

```cpp
#include <format>
#include <iostream>
#include <string_view>
#include <string>

namespace impl {
    struct Helper {
        const std::string_view s;
        Helper(const char* s, std::size_t len): s(s, len) {}
        template <typename... Args>
        std::string operator()(Args&&... args) const {
            return std::vformat(s, std::make_format_args(std::forward<Args>(args)...));
        }
    };
} // namespace impl
impl::Helper operator""_f(const char* s, std::size_t len) noexcept {
    return {s, len};
}

int main() {
    std::cout << "乐 :{} *\n"_f(5);
    std::cout << "乐 :{0} {0} *\n"_f(5);
    std::cout << "乐 :{:b} *\n"_f(0b01010101);
    std::cout << "{:*<10}"_f("卢瑟");
    std::cout << '\n';
    int n{};
    std::cin >> n;
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v<double>, n);
}
```

### 标准答案

```cpp
constexpr auto operator""_f(const char* fmt, size_t) {
    return[=]<typename... T>(T&&... Args) { return std::vformat(fmt, std::make_format_args(std::forward<T>(Args)...)); };
}
```

### 解析

我们需要使用到 C++11 用户定义字面量，`""_f` 正是[用户自定义字面量][cpp/language/user_literal]。<br/>
但**字面量运算符**（用户定义字面量所调用的函数被称为字面量运算符）的形参列表有一些限制，我们需要的是 `const char *, std::size_t` 这样的形参列表，恰好这是允许的；而字面量运算符的返回类型需要自定义，这个类型需要在内部重载 **`operator()`**，以满足上述字面量像函数一样调用的要求。

我们一步一步来：

```c++
void operator""_test(const char* str, std::size_t){
    std::cout << str << '\n';
}

"luse"_test; //调用了字面量运算符，打印 luse

std::size_t operator""_test(const char* , std::size_t len){
    return len;
}

std::size_t len = "luse"_test; //调用了字面量运算符，返回 luse 的长度 4
```

上面这段代码的两个使用示例展示了我们用户定义字面量的基本使用，尤其注意第二段，**返回值**。如果要做到像 `"xxx"_f(xxx)` 这样调用，就得在返回类型上做点手脚。

```cpp
struct X{
    std::size_t operator()(std::size_t n)const{
        return n;
    }
};

X operator""_test(const char* , std::size_t){
    return {};
}

std::cout<<"无意义"_test(1); //打印 1
```

以上这段简单的代码很好的完成了我们需要的调用形式，那么是时候完成题目要求的功能了。最简单的方式是直接使用 C++20 format 库进行格式化。

```cpp
namespace impl {
    struct Helper {
        const std::string_view s;
        Helper(const char* s, std::size_t len): s(s, len) {}
        template <typename... Args>
        std::string operator()(Args&&... args) const {
            return std::vformat(s, std::make_format_args(std::forward<Args>(args)...));
        }
    };
} // namespace impl
impl::Helper operator""_f(const char* s, std::size_t len) noexcept {
    return {s, len};
}
```

`operator""_f` 本身非常简单，只是用来把传入的参数（格式字符串）和长度，构造 `mpl::Helper` 对象再返回。`Helper` 类型使用了一个 `string_veiw` 作为数据成员，存储了格式字符串，以供后面格式化使用。

**重点只在于 `operator()`。** 它是一个变参模板，用来接取我们传入的任意类型和个数的参数，然后返回格式化后的字符串。

这里用到的是 `std::vformat` 进行格式化，它的第一个参数是格式字符串，也就是我们要按照什么样的规则去格式化；第二个参数是要格式化的参数，但是我们没有办法直接进行形参包展开，它第二个参数的类型实际上是 `std::format_args`。
我们必须使用 `std::make_format_args` 函数传入我们的参数，它会返回 `std::format_args` 类型，其实也就是相当于转换一下，合理。

不过显然标准答案不是这样的，还能简化，直接让 `""_f` 返回一个 lambda 表达式即可。

---

## `03` 实现 `print` 以及特化 `std::formatter`

日期：**`2023/7/24`** 出题人：**`mq白`**

实现一个`print`，如果你做了上一个作业，我相信这很简单。
要求调用形式为:

```cpp
print(格式字符串，任意类型和个数的符合格式字符串要求的参数)
```

```cpp
struct Frac {
   int a, b;
};
```

给出自定义类型`Frace`，要求支持

```cpp
Frac f{ 1,10 };
print("{}", f);// 结果为1/10
```

### 运行结果

```text
1/10
```

- 难度：**★★★☆☆**

禁止面向结果编程，使用宏等等方式，最多`B`（指评价），本作业主要考察和学习`format`库罢了。

提示: **`std::formatter`**

提交代码最好是网上编译了三个平台的截图，如：

![图片](image/第03题/01展示.jpg)

### [群友提交](src/群友提交/第03题)

### 标准答案

```cpp
template<>
struct std::formatter<Frac>:std::formatter<char>{
    auto format(const auto& frac, auto& ctx)const{//const修饰是必须的
        return std::format_to(ctx.out(), "{}/{}", frac.a, frac.b);
    }
};
void print(std::string_view fmt,auto&&...args){
    std::cout << std::vformat(fmt, std::make_format_args(std::forward<decltype(args)>(args)...));
}
```

我们只是非常简单的支持了**题目要求**的形式，给 `std::formatter` 进行特化，如果要支持比如那些 `{:6}` 之类的格式化的话，显然不行，这涉及到更多的操作。
简单的特化以及 [`std::formatter`](cpp/utility/format/formatter) 支持的形式可以参见[**文档**](cpp/utility/format/formatter)。
一些复杂的特化，up 之前也有写过，在 [**Cookbook**][Mq-b/Cookbook/7.6] 中，里面有对 [`std::ranges::range`][cpp/ranges/range] 和 [`std::tuple`][cpp/utility/tuple] 的特化，支持所有形式。

### 解析

实现一个 print 很简单，我们只要按第二题的思路来就行了，一个格式化字符串，用 std::string_view 做第一个形参，另外需要任意参数和个数，使用形参包即可。

```c++
void print(std::string_view fmt,auto&&...args){
    std::cout << std::vformat(fmt, std::make_format_args(std::forward<decltype(args)>(args)...));
}
```

此处我们没有显示声明模板形参，所以展开时不能使用以往的模板形参做完美转发的模板实参，但是根据形参包展开的规则。例
`args...`展开成`args1,args2,args3...`,而上式展开成

```c++
std::forward<decltype(args1)>(args1),
std::forward<decltype(args2)>(arsg2),
std::forward<decltype(args3)>(args3),... 
```

这样我们对每个应用到的参数用 decltype 取他的类型再作为完美转发的模板参数。这样调用 `vformat`,返回 string,可以使用 cout 直接输出。

而关于自定义 `std::formatter` 特化，我们需要知道的是：想要自定义 **std::formatter** 模板特化需要提供两个函数，**parse** 和 **format**。

**parse** 用来处理格式说明，并且设置相关的成员变量，对于本题我们不需要麻烦地实现此成员函数；<br/>
我们选择继承 `std::formatter<char>` 的 **parse** 函数，独立实现 **format** 函数。如果不了解此处模板特化的语法，请复习[模板特化](https://zh.cppreference.com/w/cpp/language/template_specialization)。

```c++
template<>
struct std::formatter<Frac> : std::formatter<char> {
    auto format(const auto& frac, auto& ctx)const{//const修饰是必须的
        return std::format_to(ctx.out(), "{}/{}", frac.a, frac.b);
    }
};
```

我们同样使用 **auto** 作占位符的简写函数模板，对于 **format** 函数，首个参数为我们传递的自定义类，第二个参数（**ctx**）为我们要传递给 `std::format_to` 输出迭代器的格式字符串。

在函数体中我们直接返回 `std::format_to()` 调用表达式的结果，此函数返回输出迭代器；返回值我们使用 **auto** 占位符进行返回值推导。

在函数实参中，`ctx.out()` 即为输出迭代器，第二个参数为可转换为 `std::string_view` 或 `std::wstring_view`，而转换结果是常量表达式和 Args 的合法格式字符串。本题中我们填入我们需要的形式，即 `{}/{}`。<br/>
我们想要两个参数塞进 `{}`，就如我们使用 `printf(%d,x)` 一样；最后两个参数为“需要塞进 `{}` 的值”，即要格式化的参数。

---

## `04` 给定类模板修改，让其对每一个不同类型实例化有不同 ID

日期：**`2023/7/25`** 出题人：**`Maxy`**

```cpp
#include<iostream>
class ComponentBase{
protected:
    static inline std::size_t component_type_count = 0;
};
template<typename T>
class Component : public ComponentBase{
public:
    //todo...
    //使用任意方式更改当前模板类，使得对于任意类型X，若其继承自Component

    //则X::component_type_id()会得到一个独一无二的size_t类型的id（对于不同的X类型返回的值应不同）
    //要求：不能使用std::type_info（禁用typeid关键字），所有id从0开始连续。
};
class A : public Component<A>
{};
class B : public Component<B>
{};
class C : public Component<C>
{};
int main()
{
    std::cout << A::component_type_id() << std::endl;
    std::cout << B::component_type_id() << std::endl;
    std::cout << B::component_type_id() << std::endl;
    std::cout << A::component_type_id() << std::endl;
    std::cout << A::component_type_id() << std::endl;
    std::cout << C::component_type_id() << std::endl;
}
```

### 运行结果

```text
0
1
1
0
0
2
```

- 难度:**★☆☆☆☆**

提交应当给出多平台测试结果，如图：

![图片](image/第04题/01展示.png)

### [群友提交](src/群友提交/第04题)

### 标准答案

```cpp
template<typename T>
class Component : public ComponentBase{
public:
    static std::size_t component_type_id(){
        static std::size_t ID = component_type_count++;
        return ID;
    }
};
```

分析：

我们需要实现 `Component` 的静态成员函数 `component_type_id`。这是从给出代码得知的：

```cpp
class A : public Component<A>
{};
A::component_type_id()
```

题目要求是每一个自定义类类型（假设是 X）继承 `Component<X>`，调用 `component_type_id()` 返回的是自己独一无二的 ID。其他的类型同理。

解决题目之前我们需要强调一个知识点：
> C++ 的模板不是具体类型，实例化之后才是（即**函数模板不是函数，类模板不是类**），类模板的静态成员或静态成员函数也不属于模板，而是属于**实例化后的具体类型**，我们可以用一段代码来展示结论：

```cpp
#include <iostream>

template<typename T>
struct Test{
    inline static int n = 10;
};

int main(){
    Test<int>::n = 1;
    std::cout << Test<void>::n << '\n';//10
    std::cout << Test<int>::n << '\n';//1
}
```

这段代码很轻易的就展示了**静态数据成员属于模板实例化后的具体类型**。`Test<void>::n` 和 `Test<int>::n` 不是相同的 n，并且 `Test<void>` 和 `Test<int>` 也不是一种类型（静态成员函数同理）。

所以我们的解法利用的是：不同的类型实例化 `Component` 类模板，也是不同的静态成员函数，静态成员函数里面的静态局部也都是唯一的，并且在第一次调用的时候才会初始化，后面就不会。

---

## `05` 实现 `scope_guard` 类型

日期：**`2023/7/29`** 出题人：[Da'Inihlus](https://github.com/dynilath)

要求实现 **`scope_guard`** 类型 （ 即支持传入任意可调用类型 , 析构的时候同时调用 ）。

```cpp
#include <cstdio>
#include <cassert>

#include <stdexcept>
#include <iostream>
#include <functional>

struct X {
    X() { puts("X()"); }
    X(const X&) { puts("X(const X&)"); }
    X(X&&) noexcept { puts("X(X&&)"); }
    ~X() { puts("~X()"); }
};

int main() {
    {
        // scope_guard的作用之一，是让各种C风格指针接口作为局部变量时也能得到RAII支持
        // 这也是本题的基础要求
        FILE * fp = nullptr;
        try{
            fp = fopen("test.txt","a");
            auto guard = scope_guard([&] {
                fclose(fp);
                fp = nullptr;
            });

            throw std::runtime_error{"Test"};
        } catch(std::exception & e){
            puts(e.what());
        }
        assert(fp == nullptr);
    }
    puts("----------");
    {
        // 附加要求1，支持函数对象调用
        struct Test {
            void operator()(X* x) {
                delete x;
            }
        } t;
        auto x = new X{};
        auto guard = scope_guard(t, x);
    }
    puts("----------");
    {
        // 附加要求2，支持成员函数和std::ref
        auto x = new X{};
        {
            struct Test {
                void f(X*& px) {
                    delete px;
                    px = nullptr;
                }
            } t;
            auto guard = scope_guard{&Test::f, &t, std::ref(x)};
        }
        assert(x == nullptr);
    }
}
```

### 运行结果

```text
Test
----------
X()
~X()
----------
X()
~X()
```

- 难度:**★★★★☆**（完全满足要求的情况下）

### [群友提交](src/群友提交/第05题)

### 标准答案

- 使用 `std::function` 并擦除类型

```cpp
struct scope_guard {
    std::function<void()>f;
    template<typename Func, typename...Args> requires std::invocable<Func, std::unwrap_reference_t<Args>...>
    scope_guard(Func&& func, Args&&...args) :f{ [func = std::forward<Func>(func), ...args = std::forward<Args>(args)]() mutable {
            std::invoke(std::forward<std::decay_t<Func>>(func), std::unwrap_reference_t<Args>(std::forward<Args>(args))...);
        } }{}
    ~scope_guard() { f(); }
    scope_guard(const scope_guard&) = delete;
    scope_guard& operator=(const scope_guard&) = delete;
};
```

- 使用 `std::tuple`+`std::apply`

```cpp
template<typename F, typename...Args>
    requires requires(F f, Args...args) { std::invoke(f, args...); }
struct scope_guard {
    F f;
    std::tuple<Args...>values;

    template<typename Fn, typename...Ts>
    scope_guard(Fn&& func, Ts&&...args) :f{ std::forward<Fn>(func) }, values{ std::forward<Ts>(args)... } {}
    ~scope_guard() {
        std::apply(f, values);
    }
    scope_guard(const scope_guard&) = delete;
};

template<typename F, typename...Args>//推导指引非常重要
scope_guard(F&&, Args&&...) -> scope_guard<std::decay_t<F>, std::decay_t<Args>...>;
```

![第五题答案动画](video/第五题答案动画.gif)

---

## `06` 解释 `std::atomic` 初始化

日期：**`2023/8/2`** 出题人：**`mq白`**

```cpp
#include <iostream>
#include <atomic>
int main() {
    std::atomic<int> n = 6;
    std::cout << n << '\n';
}
```

解释，为什么以上[代码](https://godbolt.org/z/sfEzP8136)在 C++17 后可以通过编译， C++17 前不行？

![图片](image/第06题/01展示.png)

- 难度:**★★★☆☆**

### [群友提交](src/群友提交/第06题)

### 标准答案

`std::atomic<int> n = 6` 中，由于 `6` 和 `std::atomic<int>` 不是同一类型（但是这里其实有一个用户定义转换序列，你可以简单的认为`6`可以隐式转换）。

即调用转换构造函数：

```cpp
constexpr atomic( T desired ) noexcept;
```

>转换构造函数也会作为用户定义的转换序列中的一部分

`6` 会调用转换构造函数，构造出一个临时的 atomic 对象用来**直接初始化 `n`**，即

```cpp
std::atomic<int> n(std::atomic<int>(6))
```

在 **C++17 之前** 的版本，理所应当应该查找检测复制/移动 构造函数，满足要求才可以通过编译。但是：

```cpp
atomic( const atomic& ) = delete;
```

实际上 atomic 的复制构造被删除（同时移动构造也被抑制生成了）。所以自然而然的不允许。

C++17 的改动是：**复制消除变为强制要求**。
纯右值表达式作为构造对象的参数，不会再调用移动构造，也不会去检测，而是原位构造。
>说句题外话，C++17后纯右值不可能再调用移动构造。没有移动构造或者复制构造不影响使用同类型纯右值初始化对象，如 `X x{X{}}` ，即使移动/复制构造函数**都被 delete**，也无所谓，[`code`](https://godbolt.org/z/Kdncxcc3o)。

---

## `07` `throw new MyException`

日期：**`2023/8/6`** 出题人：**`mq白`**

给出代码:

```cpp
struct MyException :std::exception {
 const char* data{};
 MyException(const char* s) :data(s) { puts("MyException()"); }
 ~MyException() { puts("~MyException()"); }
 const char* what()const noexcept { return data; }
};
void f2() {
 throw new MyException("new Exception异常....");
}
int main(){
    f2();
}
```

> 灵感来源自 **Java** 人写 **C++**

在 `main` 函数中自行修改代码，接取 `f2()` 函数抛出的异常（`try catch`）。

### 运行结果

```text
MyException()
new Exception异常....
~MyException()
```

- 难度: **★☆☆☆☆**

> 某些 IDE 或者平台可能会将打印的异常信息标为红色放到第一行，即
> new Exception 异常.... 这句话也可能在第一行（一般终端运行不会，默认 vs 也无此功能）

### [群友提交](src/群友提交/第07题)

### 标准答案

```cpp
int main() {
    try {
        f2();
    }
    catch (std::exception* e){
        std::cerr << std::unique_ptr<std::exception>(e)->what() << '\n';
    }
}
```

实际上本题是用来讽刺将 Java 的写法带入到其他语言中，也就是很经典的：**Java 人写什么都是 Java**。
只是看我们这道题，实际上你非要说 `new` 有什么不好，倒也没什么非常不行的地方，只是，没有理由自己多写一个 `delete` 表达式（或者包个智能指针）。
> 我希望不要有人开始幻想：`throw new MyException("new Exception 异常....")`因为是 `throw` 一个指针类型，所以按指针传递，效率更高。不要让我看到这种逆天想法。如果你想到这一点，那不妨思考一下，构造临时对象的开销，以及使用 `new` 表达式？说实话挺无聊的问题，只是防止有人想到这些点，以及抬杠罢了。

---

## `08` 定义`array`推导指引

日期：**`2023/8/12`** 出题人：**`mq白`**

给出代码:

```cpp
template<class Ty,size_t size>
struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};
int main() {
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr) {
        std::cout << i << ' ';
    }
}
```

要求**自定义推导指引**，不更改已给出代码，使得代码成功编译并满足运行结果。

> 提示：可参考[`std::array`](https://zh.cppreference.com/w/cpp/container/array)

### 运行结果

```text
1 2 3 4 5 
```

- 难度: **★★★☆☆**

### [群友提交](src/群友提交/第08题)

### 标准答案

```cpp
template<class Ty, class...Args>
    requires (std::same_as<Ty, Args>&&...)//不会这个C++20约束以及里面折叠表达的用法也可以不用
array(Ty, Args...) -> array<Ty, 1 + sizeof...(Args)>;
```

本题的目的如你所见主要考察的是 **C++17 用户定义类模板推导指引**。但是我更多的其实我想表达的是：
定义类模板推导指引和构造函数没什么直接的关联；如题目所示，我们的 `array` 是一个聚合类型，压根没有显式的用户定义构造函数，**没有显示的构造函数并不影响我们使用自定义的用户推导指引**。

> 如此强调只是因为大部分人对此有很多错误认知

推导指引实际上是按照你构造器传入的东西，来进行的这种推导，我觉得我视频里说的已经很清楚了。

---

## `09` 名字查找的问题

日期：**`2023/8/15`** 出题人：**`mq白`**

```cpp
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
    }
    void t2()const {
        f();
    }
};

int main() {
    Y<void>y;
    y.t();
    y.t2();
}
```

给出以上代码，要求解释其运行结果。

### 运行结果

```text
X
全局
```

- 难度: **★★★☆☆**

> 本问题堪称经典，**在某著名 template 书籍也有提过**（虽然它完全没有讲清楚）。
> 并且从浅薄的角度来说，本题也可以让你向其他人证明加 **`this`** 访问类成员，和不加，是有很多区别的。

提示：[**名字查找**](https://zh.cppreference.com/w/cpp/language/lookup)

### [群友提交](src/群友提交/第09题)

### 标准答案

根据我们的提示 **名字查找**，我看大家多少都还是能查到一些东西。我们慢慢说吧，首先名字查找分为：**有限定** 名字查找，**无限定** 名字查找。

> 有限定名字查找指？
>  
> 出现在作用域解析操作符 `::` 右边的名字是限定名（参阅有限定的标识符）。 限定名可能代表的是：
>  
> - 类的成员（包括静态和非静态函数、类型和模板等）
> - 命名空间的成员（包括其他的命名空间）
> - 枚举项

如果 `::` 左边为空，那么查找过程只会考虑全局命名空间作用域中作出（或通过 using 声明引入到全局命名空间中）的声明。

```cpp
this->f();
```

那么显然，这个表达式 **不是有限定名字查找**，那么我们就去[无限定名字查找](https://zh.cppreference.com/w/cpp/language/unqualified_lookup)中寻找答案。

我们找到**模板定义**：

> 对于在模板的定义中所使用的**非待决名**，当**检查该模板的定义时将进行无限定的名字查找**。在这个位置与声明之间的绑定并不会受到在实例化点可见的声明的影响。
> 而对于在模板定义中所使用的**待决名**，**它的查找会推迟到得知它的模板实参之时**。
> 此时，ADL 将同时在模板的定义语境和在模板的实例化语境中检查可见的具有外部连接的（C++11 前）函数声明，而非 ADL 的查找只会检查在模板的定义语境中可见的具有外部连接的（C++11 前）函数声明。
>（换句话说，在模板定义之后添加新的函数声明，除非通过 ADL 否则仍是不可见的。）
> 如果在 ADL 查找所检查的命名空间中，在某个别的翻译单元中声明了一个具有外部连接的更好的匹配声明，或者如果当同样检查这些翻译单元时其查找会导致歧义，那么行为未定义。
> 无论哪种情况，如果某个基类取决于某个模板形参，**那么无限定名字查找不会检查它的作用域（在定义点和实例化点都不会）**。

原文很长，但是看我们加粗的也足够了。如果你没有学习过[待决名](https://zh.cppreference.com/w/cpp/language/dependent_name)，那这又会引入一个问题了。

我们这里简单描述一下吧：

`this->f()` 是一个待决名，这个 `this` 依赖于模板 `X`。

所以，我们的问题可以解决了吗？

1. `this->f()` **是待决名**，所以它的查找会推迟到得知它模板实参之时（届时可以确定父类是否有 `f` 函数）。
2. `f()` **是非待决名**，检查该模板的定义时将进行无限定的名字查找（无法查找父类的定义），按照正常的查看顺序，先类内（查找不到），然后全局（找到）。

>补充：如果是 `msvc` 的某些早期版本，或者 c++版本设置在 C++20之前，会打印 `X` `X`。这是因为 `msvc`不支持 [`Two-phase name lookup`](https://learn.microsoft.com/zh-cn/archive/blogs/c/msvc%E5%B7%B2%E7%BB%8F%E6%94%AF%E6%8C%81two-phase-name-lookup)。
详细的可以看看文档。实测 `Microsoft Visual Studio 17.6.4` 设置 `C++20` 之前的版本都无法得到正确结果。

---

## `10` 遍历任意类数据成员

出题人：`mq白` 日期：`2023/8/18`

题目的要求非常简单，在很多其他语言里也经常提供这种东西（一般是反射）。
但是显而易见 `C++` 没有反射。

我们给出代码：

```cpp
int main() {
    struct X { std::string s{ " " }; }x;
    struct Y { double a{}, b{}, c{}, d{}; }y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto& member) {
        std::cout << member << ' ';
    };
    for_each_member(x, print);
    for_each_member(y, print);
}
```

要求自行实现 `for_each_member` 以及 `size` 模板函数。
要求支持任意自定义类类型（**聚合体**）的数据成员遍历（**聚合体中存储数组这种情况不需要处理，具体原因看最后的[补充说明](#补充说明)**）。
这需要打表，那么我们的要求是支持聚合体拥有 `0` 到 `4` 个数据成员的遍历。
>如果从来没有接触过，那这道题有相当的难度，可以等 **标准答案** 以及 **视频**。或者去网络上查找这方面的知识。

### 运行结果

```text
1
4
  0 0 0 0
```

- 难度: **★★★★☆**

> 第二个四星题目

提示：[学习](https://akrzemi1.wordpress.com/2020/10/01/reflection-for-aggregates/)

### [群友提交](src/群友提交/第10题)

### 标准答案

分为 **`C++20`** 的写法和 **`C++17`** 的写法。
`C++20` 得益于 [**`requires`**](https://zh.cppreference.com/w/cpp/language/requires) 表达式，可以简化不少地方。主要在于 **获取聚合类型的成员个数**，即 `size`函数的实现方式的不同。

其他的，诸如用于在不求值语境的 `init` 辅助类，或者用于遍历的 `for_each_member` 函数，在 `C++17` 和 `C++20` 没什么区别。

我们统一先放标准答案的代码再一一讲解。

#### `C++17` 写法

```cpp
#include <iostream>
#include<type_traits>

struct init {
    template <typename T>
    operator T(); // 无定义 我们需要一个可以转换为任何类型的在以下特殊语境中使用的辅助类
};

template<unsigned I>
struct tag :tag<I - 1> {};//模板递归展开 继承 用来规定重载的匹配顺序 如果不这么写，匹配是无序的
template<>
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
    static_assert(std::is_aggregate_v<T>);//检测是否为聚合类型
    return size_<T>(tag<4>{});//这里就是要求从tag<4>开始匹配，一直到tag<0>
}

template <typename T, typename F>
void for_each_member(T const& v, F&& f) {
    static_assert(std::is_aggregate_v<T>);//检测是否为聚合类型

    if constexpr (size<T>() == 4u) {//使用C++17的编译期if和结构化绑定来遍历
        const auto& [m0, m1, m2, m3] = v;
        f(m0); f(m1); f(m2); f(m3);
    }
    else if constexpr (size<T>() == 3u) {
        const auto& [m0, m1, m2] = v;
        f(m0); f(m1); f(m2);
    }
    else if constexpr (size<T>() == 2u) {
        const auto& [m0, m1] = v;
        f(m0); f(m1);
    }
    else if constexpr (size<T>() == 1u) {
        const auto& [m0] = v;
        f(m0);
    }
}

int main() {
    struct X { std::string s{ " " }; }x;
    struct Y { double a{}, b{}, c{}, d{}; }y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto& member) {
        std::cout << member << ' ';
    };
    for_each_member(x, print);
    for_each_member(y, print);
}
```

[代码运行](https://godbolt.org/z/3GY5ah88G)

我们稍微聊一下那个模板类 `tag` 的作用，它那里是一个模板递归继承，注释也写了，用来规定重载决议匹配顺序，那么原理是什么呢？

```cpp
#include <iostream>

struct X{};
struct Y:X{};
struct G:Y{};

struct X2:X{};

void f(X) { puts("X"); }
void f(Y) { puts("Y"); }

void f2(X) { puts("X"); }

int main(){
    f(G{});//G的父类是Y，重载决议优先选择f(Y)
    f2(G{});//但是实际上使用X一样可以，当没有更匹配的重载，重载决议会选择到f2(X)
}
```

[运行结果](https://godbolt.org/z/nGvjqo9bq)

```text
X
Y
```

我们继续看 `tag` 的代码

```cpp

template<unsigned I>
struct tag : tag<I - 1> {};
template<>
struct tag<0> {};
```

假设我们实例化了 `tag<4>` ，那么相当于

```cpp
template<4>
struct tag :tag<3> {};
```

然后递归，以此类推，最终到 `tag<0>` ，结束。

也就是说 `tag<4>` 继承自 `tag<3>` ，`tag<2>` 继承自 `tag<1>` ，`tag<1>` 继承自 `tag<0>`。

然后看到 `size_<T>(tag<4>{});` 以及 `size_` 模板的几个重载。

结合我们之前说的，我们可以知道，这样就是约束匹配顺序从

```cpp
template <typename T>
constexpr auto size_(tag<4>) -> decltype(T{ init{}, init{}, init{}, init{} }, 0u);
```

开始，一直到

```cpp
template <typename T>
constexpr auto size_(tag<0>) -> decltype(T{}, 0u);
```

也就是从大到小，获取传入的聚合类型的成员个数。

---

#### `C++20` 写法

```cpp
#include <iostream>
#include <type_traits>

struct init {
    template <typename T>
    operator T(); // 无定义 我们需要一个可以转换为任何类型的在以下特殊语境中使用的辅助类
};

template <typename T>
consteval size_t size(auto&&...Args) {
    if constexpr (!requires{T{ Args... }; }) {
        return sizeof...(Args) - 1;
    }
    else {
        return size<T>(Args..., init{});
    }
}

template <typename T, typename F>
void for_each_member(T const& v, F&& f) {//和C++17的写法一毛一样
    static_assert(std::is_aggregate_v<T>);//检测是否为聚合类型

    if constexpr (size<T>() == 4u) {//使用C++17的编译期if和结构化绑定来遍历
        const auto& [m0, m1, m2, m3] = v;
        f(m0); f(m1); f(m2); f(m3);
    }
    else if constexpr (size<T>() == 3u) {
        const auto& [m0, m1, m2] = v;
        f(m0); f(m1); f(m2);
    }
    else if constexpr (size<T>() == 2u) {
        const auto& [m0, m1] = v;
        f(m0); f(m1);
    }
    else if constexpr (size<T>() == 1u) {
        const auto& [m0] = v;
        f(m0);
    }
}

int main() {
    struct X { std::string s{ " " }; }x;
    struct Y { double a{}, b{}, c{}, d{}; }y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';

    auto print = [](const auto& member) {
        std::cout << member << ' ';
    };
    for_each_member(x, print);
    for_each_member(y, print);
}
```

[代码运行](https://godbolt.org/z/dbTPGhvKd)

我们来描述一下`size`模板函数的实现。
>他的设计非常的巧妙，如你所见，它是一个递归函数，还是编译期的递归，使用到了 **编译期 `if`** ；并且这个函数是以 [`consteval`](https://zh.cppreference.com/w/cpp/language/consteval) 修饰 ，是立即函数，即必须在编译期执行，**产生编译时常量**。

好，我们正式进入这个函数。首先看到函数前两行，模板和 `C++20` 简写模板，这是一个形参包，万能引用。`typename T` 的 `T` 是指代外部传入的，需要获取成员个数的聚合体类型。

```cpp
template <typename T>
consteval size_t size(auto&&...Args)
```

然后是

```cpp
if constexpr (!requires{T{ Args... }; })
```

这句话表示的是：传入的聚合体类型如果可以 `T{ Args... }`这样构造，那就不进入分支。注意这个 **`!`**。

我们先重复一下聚合体的匹配，假设有一个聚合类型 `X`，如下所示：

```cpp
struct X{
    int a,b,c;
};
```

它支持几个参数的初始化呢？如下所示

```cpp
X x0{};        //ok
X x1{1};       //ok
X x2{1,2};     //ok
X x3{1,2,3};   //ok
X x4{1,2,3,4}; //error
```

**上面这段例子非常重要**。显然，只要传入的参数类型符合要求，并且**不超过其成员个数**即可。

我们再给一段代码：

```cpp
#include <iostream>

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

struct X{
    int a,b,c;
};

int main(){
    std::cout << size<X>() << '\n';//3
}
```

我们一步一步带入解释，为什么会打印 **3**。

1. 进入 `size` 函数，`T` 是 `X`类型，形参包 `Args` 为**空**。
2. 编译期 `if` 中，条件表达式等价于 `! requires{ X{}; }` 。显然 `X{}` 符合语法，`requires` 表达式会返回 `true` 但是有 **`!`**，那就是  **`false`**。不进入这个分支。
3. 进入 `else` ，直接相当于 `return size<X>(init{})` 。
4. **第二次** 进入 `size` 函数，此时形参包 `Args` 有**一个**参数 `init`。
5. 编译期 `if` 中，条件表达式等价于 `! requires{ X{ init{} }; }`。显然 `X{ init{} }` 符合语法。同 `2` 返回 **`false`**，不进入分支。
6. 进入 `else` ，直接相当于 `return size<X>(init{},init{})`。
7. **第三次** 进入 `size` 函数，此时形参包 `Args` 有**两个**参数 `init`。
8. 编译期 `if` 中，条件表达式等价于 `! requires{ X{ init{},init{} } }`。显然同 `2` `5` 返回 **`false`**，不进入分支。
9. 进入 `else`，直接相当于 `return size<X>(init{},init{},init{})`。
10. **第四次** 进入 `size` 函数，此时形参包 `Args` 有**三个**参数 `init`。
11. 编译期 `if` 中，条件表达式等价于 `! requires{ X{ init{},init{},init{} } }`，显然同 `2` `5` `8` 返回 **`false`**，不进入分支。
12. 进入 `else`，直接相当于 `return size<X>(init{},init{},init{},init{})`。
13. **第五次** 进入 `size` 函数，此时形参包 `Args` 有**四个**参数 `init`。（**注意，重点要来了，`X` 类型只有三个成员**）
14. 编译期 `if` 中，条件表达式等价于 `! requires{ X{ init{},init{},init{},init{} } }`，即 `X{ init{},init{},init{},init{} }`不符合语法（`X` 类型只有三个成员）。
所以 `requires` 表达式返回 `false`，然后因为 **`!`** ，表达式结果为 **`true`**，进入分支。
15. `return sizeof...(Args) - 1;` 注意，我们说了，第五次进入的时候，形参包 `Args` 已经有四个参数，所以`sizeof...(Args)`会返回 `4` ，再 `-1`，也就是  **`3`**。**得到最终结果**。

到此，我们介绍完了 `C++20`写法的 获取聚合类型的 `size` 函数。

至于 `for_each_member` 没必要再介绍，很普通简单的分支逻辑而已，只不过是用了编译期的分支。

### 无法处理引用类型以及不可移动类型

先前的代码没办法处理引用类型和不可移动类型，如下：

```cpp
struct init {
    template <typename T>
    operator T(); 
};

template <typename T>
consteval size_t size(auto&&...Args) {
    if constexpr (!requires{T{ Args... }; }) {
        return sizeof...(Args) - 1;
    }
    else {
        return size<T>(Args..., init{});
    }
}

struct X{
    int& v1,v2,v3; // 换成 int&& 也都同理
};

int main(){
    std::cout << size<X>() << '\n'; // 18446744073709551615
}
```

```cpp
struct init {
    template <typename T>
    operator T(); 
};

template <typename T>
consteval size_t size(auto&&...Args) {
    if constexpr (!requires{T{ Args... }; }) {
        return sizeof...(Args) - 1;
    }
    else {
        return size<T>(Args..., init{});
    }
}

struct X{
    std::mutex m;   // 不可移动不可复制
    int v1,v2,v3;
};

int main(){
    std::cout << size<X>() << '\n'; // 0
}
```

换自己写的：

```cpp
struct init {
    template <typename T>
    operator T(); 
};

template <typename T>
consteval size_t size(auto&&...Args) {
    if constexpr (!requires{T{ Args... }; }) {
        return sizeof...(Args) - 1;
    }
    else {
        return size<T>(Args..., init{});
    }
}

struct Y
{
    Y() {}
    Y(const Y&) {}
    Y(Y&&) = delete;
};

struct X{
    Y y1,y2,y3,y4;
};

int main(){
    std::cout << size<X>() << '\n';
}
```

懒得改。

### 补充说明

我们给出的 `C++20` 或 `C++17` 的 `size` 的实现是有问题的，简单的说，**它没办法处理聚合类型存储数组的问题**，包括引用、不可移动类型。在题目开头我们也说了。
我们拿 [`boost::pfr`](https://www.boost.org/doc/libs/1_82_0/doc/html/boost_pfr/tutorial.html) 的行为作为参考，我们采用1.82.0版本。

参见 [示例代码](src/群友提交/第10题/with_boost_pfr.md)

---

## `11` `emplace_back()` 的问题

日期：**`2023/8/20`** 出题人：[**`jacky`**](https://github.com/rsp4jack)

思考：以下代码为什么在 `C++20` 以下的版本中无法成功编译，而在 `C++20` 及以后却可以？

```cpp
#include <vector>

struct Pos {
    int x;
    int y;
};

int main(){
    std::vector<Pos> vec;
    vec.emplace_back(1, 5);
}
```

- 难度:**★★☆☆☆**

### [群友提交](src/群友提交/第11题)

### 标准答案

你在使用 `gcc` 并且设置标准在 C++20 之前，会得到 [编译器的提示信息](https://godbolt.org/z/rdjjYEcje)

```text
error: new initializer expression list treated as compound expression [-fpermissive]
  187 |         { ::new((void *)__p) _Up(std::forward<_Args>(__args)...); }
      |           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```

实际上 `std::vector<T>::emplace_back()` 最终会使用类似如下的代码构造对象：

```cpp
::new (static_cast<void*>(p)) T(std::forward<Args>(args)...)//也就是完美转发+布置new
```

实际上是因为在 C++20 中增加了 “**括号形式的聚合初始化**”（用词并不准确，当字面意思凑合理解即可）。按照[文档](https://zh.cppreference.com/w/cpp/language/direct_initialization)的描述

>如果目标类型是（可能有 cv 限定）的 **聚合类**，则 **按聚合初始化** 中所述进行初始化，但允许窄化转换，不允许指派初始化器，不延长引用所绑定到的临时量的生存期，不进行花括号消除，并值初始化任何无初始化器的元素。

我们不需要关注那么多，我们用一段代码来演示：

```cpp
struct Pos {
    int x;
    int y;
};

int main(){
    Pos p(0,0);//C++20起允许
}
```

所以说白了，就是 `T(std::forward<Args>(args)...)` 这里用的小括号进行初始化，直到 C++20才允许聚合类型使用小括号初始化。

---

## `12` 实现`make_vector()`

日期：**`2023/8/28`** 出题人：[**`jacky`**](https://github.com/rsp4jack)

请实现函数 `make_vector(...)`，使以下代码编译通过（**C++20**）：

```cpp
#include <cstdio>
#include <vector>

inline void dbg(const char* msg)
{
    std::puts(msg);
    std::fflush(stdout);
}

struct X {
    X() noexcept
    {
        dbg("X()");
    };

    ~X() noexcept
    {
        dbg("~X()");
    };

    X(const X&)
    {
        dbg("X(const X&)");
    }

    X(X&&) noexcept
    {
        dbg("X(X&&)");
    }
};

void test()
{
    static_assert(requires {
        {
            make_vector(std::vector{1, 2, 3})
        } -> std::same_as<std::vector<std::vector<int>>>;
        {
            make_vector(1, 2, 3)
        } -> std::same_as<std::vector<int>>;
        make_vector(1, 2, 3).size() == 3;
    });
    X    x1;
    X    x2;
    auto vec = make_vector(x1, std::move(x2));
}

int main()
{
    test();
    dbg("test end");
}
```

### [运行结果](https://godbolt.org/z/qYn74qGee)

```text
X()
X()
X(const X&)
X(X&&)
X(const X&)
X(const X&)
~X()
~X()
~X()
~X()
~X()
~X()
test end
```

- 难度:**★★★☆☆**

### [群友提交](src/群友提交/第12题)

答题者：[`yuzhiy`](src/群友提交/第12题/yuzhiy.cpp)

```cpp
template<typename...Args>
auto make_vector(Args&&...args)->decltype(auto){
    return std::vector({std::forward<Args>(args)...});
}
```

> 没啥问题，就是 `auto` 占位，后置返回类型 `decltype(auto)` 没意义。

答题者：[`Matrix-A`](src/群友提交/第12题/Matrix-A.cpp)

```cpp
auto make_vector(auto&&... args) {
    std::vector<std::common_type_t<decltype(args)...>> temp;
    (temp.emplace_back(std::forward<decltype(args)>(args)),...);
    return temp;
}
```

> 用 `emplace_back()` 比直接往 `vector` 的初始化器里面传入参数要少拷贝。可[自行测试](https://gcc.godbolt.org/z/3bo7oKfEq)。

### 标准答案

```cpp
template<typename T,typename...Args>
constexpr auto make_vector(T&&t,Args&&...args) {
    return std::vector<std::remove_cvref_t<T>>{ std::forward<T>(t), std::forward<Args>(args)... };
}
```

> 这种写法纯属为了解决下面解法提到的问题（即推导问题）

```cpp
template<typename...Args>
constexpr auto make_vector(Args&&...args) {
    return std::vector({ std::forward<Args>(args)... });
}
```

事实上大多数人看到这段代码会觉得很简单，就这么一行核心代码。实际上这里有很多坑，如果你没有自己写过不会注意到，即使你自己写过，大多人也并不明白。

即：**为什么需要 `std::vector({})` 这种形式？如果我不这么做呢？只用 `()` 或者 `{}` 呢？有什么替代方式吗？**

我们如果去掉外面的 `()` 进行编译，会发生一个**编译错误**，编译器提示我们是`static_assert` 中的 `requires` 表达式出错了，但是看不出是哪个错误。

我们去掉 `requires` 表达式中的：

```cpp
{
    make_vector(std::vector{1, 2, 3})
} -> std::same_as<std::vector<std::vector<int>>>;
```

这段代码，发现就可以通过编译了。那么问题很简单了，就是：
`make_vector(std::vector{1, 2, 3})` 这段代码无法得到 `std::vector<std::vector<int>>` 类型（前提是我们去掉了 `()` ）。

那么这个问题的本质是什么？

```cpp
//C++20
std::vector v{std::vector{1,2,3}};  // std::vector<int>
```

就这么简单，这就是这个问题表面上的本质，`vector` 再这种情况下也会得不到我们想要的类型。

我们接下来需要解释为什么 `vector` 会有这样的问题，并且，如果是：

```cpp
std::vector v{std::vector{1},std::vector{2}}; //就能得到std::vector<std::vector<int>>
```

我们写一个最小的复现 demo 即可

```cpp
#include <cstdio>
#include <vector>

template<typename T>
struct Test {
    Test() {}
    Test(std::initializer_list<T>) { puts("被调用"); }
    Test(const Test<T>&) { puts("复制构造"); }
};

template<typename T>
Test(std::initializer_list<T>) -> Test<T>;

int main() {
    Test<int> a;
    Test t{ a,a }; //被调用         会推导为Test<Test<int>>
    Test t2{ a };  //复制构造       会推导为Test<int>
}
```

#### [运行结果](https://godbolt.org/z/jvKE7Mqhq)

```text
复制构造
复制构造
被调用
复制构造
```

根据运行结果我们可以知道，`std::initializer_list` 的构造函数版本只被调用了一次，是 `Test t{ a,a };` 。

`Test t2{ a };` 并没有调用 `std::initializer_list` 的构造函数。

不用感到奇怪，这理所应当。

**当从类型为正在构造的类模板的特化或特化子级的单个元素进行初始化时，复制构造函数优先于列表构造函数。**

这就可以解释为什么直接 `{}` 不行，`({})` 的话就很明确了，只能调用 `initializer_list` 的构造函数。

包括可以解释以下给出的**错误**写法：

```cpp
template<typename... Args>
auto make_vector(const Args&... elems){
    return std::vector{elems...};
}
auto v2 = make_vector(std::vector{1,2,3});  // std::vector<int>
```

可参见[文档](https://oleksandrkvl.github.io/2021/04/02/cpp-20-overview.html#fix-init-list-ctad)描述。

---

## `13` 关于 `return std::move`

日期：`2023/9/6` 出题人：`mq白`

我们会给出三段使用到了 `return std::move` 代码。

**解释说明这些代码是否有问题，问题在哪，或者没问题，那么为什么要这样使用。**

1. 全局函数，返回局部对象，使用 `std::move`。

    ```cpp
    #include<iostream>

    struct X{//后续代码不再重复X类
        X() { puts("X()"); }
        X(const X&) { puts("X(const X&)"); }
        X(X&&)noexcept { puts("X(X&&)"); }
        ~X() { puts("~X()"); }
    };

    X f(){
        X x;
        return std::move(x);
    }

    int main(){
        X x = f();
    }
    ```

2. 全局函数，返回局部的引用，使用 `std::move`。

    ```cpp
    X&& f(){
        X x;
        return std::move(x);
    }
    ```

3. 类中成员函数，返回数据成员，使用 `std::move`。

    ```cpp
    struct Test {
        X x;
        X f() {
            return std::move(x);
        }
    };
    ```

- 难度:**★★★☆☆**

### [群友提交](src/群友提交/第13题)

### 标准答案

看[博客](https://zhuanlan.zhihu.com/p/654113232)或[视频](https://www.bilibili.com/video/BV1SG41197RC/)都有讲。

1. 函数局部的 `x` 对象是隐式可移动实体，可以调用移动构造，显式 `std::move` 毫无意义，甚至会影响 `NRVO` 优化。
2. 返回局部的引用，这是悬垂的。
3. 类的数据成员不是隐式可移动实体,如果不 `std::move`，而是直接 `return x`，重载决议不会选择移动构造。所以有意义。

## `14` 以特殊方法修改命名空间中声明的对象

日期：`2023/12/5` 出题人：`mq白`

给出以下代码，不得修改，要求不得以

- ss::a

- using namespace ss

- namespace x = ss，x::a
  
- using ss::a

- 直接在 ss 命名空间中通过声明引用或指针指向 a ，然后再去修改 a

这些方式去修改命名空间 `ss` 中的对象 **a**，并且满足运行结果。

需要真的修改了 a，而不是别的东西，诸如更改入口函数等。

**不要求你的做法完全符合标准**。

```cpp
#include<iostream>

namespace ss {
    int a = 0;
}

int main() {
    // todo..
    std::cout << ss::a << '\n'; 
}
```

### 运行结果

```txt
100
```

- 难度: **★★★★☆**

> 本题其实是“**魔法**”题，并不是常规的那些，你可能会对本题的存在一些质疑，认为这种东西毫无价值，但这恰恰考察的是别的基础知识，如**符号修饰、链接、内存布局**。
> 越是写的奇特，诡异，**往往令人印象深刻**，这也正是教学的方式。
> 举个例子，别人和你说“C/C++ 的 #include 的本质是复制”，你可能没什么感觉，但是如果直接写一段代码呢？

```cpp
int arr[] = {
#include"1.txt"
};
```

> **即使这种形式基本上没有任何的实际用途和价值，但是它能教会各位，让各位印象深刻，那也足够了。**

### [群友提交](src/群友提交/第14题)

答题者：[**`Matrix-A`**](src/群友提交/第14题/Matrix-A.cpp)

```cpp
#include<iostream>
#include<cstdint>

namespace ss {
    int a = 0;
}

#if defined(_MSC_VER)
#define MAIN_ATTRIBUTES __pragma(optimize("", off))
#elif defined(__clang__)
#define MAIN_ATTRIBUTES [[clang::optnone]]
#else
#define MAIN_ATTRIBUTES [[gnu::optimize(0)]]
#endif

int b = 0;
MAIN_ATTRIBUTES
int main() {
    *(int*)((uint8_t*)&b - sizeof(int)) = 100;
    *(int*)((uint8_t*)&b + sizeof(int)) = 100;
    std::cout << ss::a << '\n';
}
```

### 标准答案

#### 利用符号来解决

```cpp
#include<iostream>

extern "C"{
    namespace ss {
        int a = 0;
    }
}

extern "C" int a;

int main() {
    a = 100;
    std::cout << ss::a << '\n'; 
}
```

> 利用符号解决，[实测](https://godbolt.org/z/os3WM4b9W)三平台通用（但显然不是标准规定的）。

`extern "C"{ }` 包含了命名空间 `ss` ，让对象 `a` 没有了额外的符号修饰，就是单纯的 `a`，如果不使用 `extern "C"` 包含，`ss::a` 的符号得是 **`_ZN2ss1aE`**。

全局作用域的 `extern "C" int a;` 使用 `extern "C"` 能让对象 `a` 没有额外的符号修饰。也就是操作这个 `a` 等价于 `ss::a`，因为他们**编译后的符号是一样的**。

也可以使用一些不太高明的方式，不需要用到 `extern "C"`

```cpp
#include<iostream>

int a = 0;

namespace ss{
    int a = 0;
}

extern int _ZN2ss1aE;

int main(){
    _ZN2ss1aE = 100;
    std::cout<<ss::a<<'\n';
} 
```

不过这种方式是写死的，并且这只支持 `gcc` 以及 `clang`，更多的解释看之前的[博客](https://zhuanlan.zhihu.com/p/667571439)。

<br>

当然了，我们也可以利用预处理器，来进行兼容。**原理**：

- Itanium ABI 上变量 ss::a 的重整名为 _ZN2ss1aE。
- 在 MSVC 上需要用 #pragma 告诉链接器使用 MSVC ABI 的重整名 ?a@ss@@3HA。

```cpp
#include<iostream>

namespace ss {
    int a = 0;
}

int main() {
    extern int _ZN2ss1aE;
#ifdef _MSC_VER
#pragma comment(linker, "/alternatename:?_ZN2ss1aE@@3HA=?a@ss@@3HA")
#endif
    _ZN2ss1aE = 100;
    std::cout << ss::a << '\n';
}
```

> 来源：[聚聚](https://github.com/Mq-b/Loser-HomeWork/pull/194)。

#### 直接修改内存

```cpp
#include<iostream>
#include<cstdint>

namespace ss {
    int a = 0;
}

int b = 0;

int main() {
    *(int*)((uint8_t*)&b - sizeof(int)) = 100;
    *(int*)((uint8_t*)&b + sizeof(int)) = 100;
    std::cout << ss::a << '\n';
}
```

没啥说的，直接 UB 越界修改内存就完事了，在全局声明一个 `b` 只是用来做基地址进行偏移罢了。

存在两个修改的原因是因为 ss::a 和 b 的前后顺序是不确定的，至少 msvc 和（clang、gcc）是不一样的，可能和系统有关。

#### 利用名字查找规则

```cpp
#include<iostream>

namespace ss {
    int a = 0;
}

namespace ss {
    extern int b;
}

int ss::b = a = 100;

int main() {
    std::cout << ss::a <<'\n';
}
```

> 来源：[mq 败](https://github.com/autobotoptimusprime)。

当某个命名空间的成员变量在该命名空间外被定义时，该定义中用到的名字的查找会以与在命名空间之内使用的名字相同的方式进行。

参见[文档](https://zh.cppreference.com/w/cpp/language/unqualified_lookup#.E5.9C.A8.E5.91.BD.E5.90.8D.E7.A9.BA.E9.97.B4.E5.A4.96.E8.BF.9B.E8.A1.8C.E5.AE.9A.E4.B9.89)。

## `15` 表达式模板

日期：`2024/1/7` 出题人：[`Matrix-A`](https://github.com/Matrix-A)（[#159](https://github.com/Mq-b/Loser-HomeWork/issues/159)）

1. 使用**表达式模板**补全下面的代码，实现表达式计算；
2. 指出**表达式模板**和 STL Ranges 库中哪些**视图**类似，并指出它们的异同和优缺点。

```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

// 为std::vector增加一个自定义的赋值函数
template <typename T>
    requires std::disjunction_v<std::is_integral<T>, std::is_floating_point<T>>
class vector : public std::vector<T> {
public:
    using std::vector<T>::vector;
    using std::vector<T>::size;
    using std::vector<T>::operator[];
    template <typename E>
    vector<T>& operator=(const E& e)
    {
        const auto count = std::min(size(), e.size());
        this->resize(count);
        for (std::size_t idx { 0 }; idx < count; ++idx) {
            this->operator[](idx) = e[idx];
        }
        return *this;
    }
};

/*
// 实现表达式模板类及相关函数
template<...>
struct vector_expr {

};

// operator+
// operator-
// operator*
// operator/
*/

int main()
{
    auto print = [](const auto& v) {
        std::ranges::copy(v, std::ostream_iterator<std::ranges::range_value_t<decltype(v)>> { std::cout, ", " });
        std::cout << std::endl;
    };
    const vector<double> a { 1.2764, 1.3536, 1.2806, 1.9124, 1.8871, 1.7455 };
    const vector<double> b { 2.1258, 2.9679, 2.7635, 2.3796, 2.4820, 2.4195 };
    const vector<double> c { 3.9064, 3.7327, 3.4760, 3.5705, 3.8394, 3.8993 };
    const vector<double> d { 4.7337, 4.5371, 4.5517, 4.2110, 4.6760, 4.3139 };
    const vector<double> e { 5.2126, 5.1452, 5.8678, 5.1879, 5.8816, 5.6282 };

    {
        vector<double> result(6);
        for (std::size_t idx = 0; idx < 6; idx++) {
            result[idx] = a[idx] - b[idx] * c[idx] / d[idx] + e[idx];
        }
        print(result);
    }
    {
        vector<double> result(6);
        result = a - b * c / d + e; // 使用表达式模板计算
        print(result);
    }
    return 0;
}
```

### 运行结果

```txt
4.73472, 4.05709, 5.038, 5.08264, 5.73076, 5.18673,
4.73472, 4.05709, 5.038, 5.08264, 5.73076, 5.18673,
```

- 难度: 待定

学习链接：

- [Wikipedia - Expression templates](https://en.wikipedia.org/wiki/Expression_templates)
- [我们不需要臭名昭著的表达式模板（英文）](https://gieseanw.wordpress.com/2019/10/20/we-dont-need-no-stinking-expression-templates/)
- [C++语言的表达式模板：表达式模板的入门性介绍](https://blog.csdn.net/magisu/article/details/12964911)
- [std::valarray](https://zh.cppreference.com/w/cpp/numeric/valarray) 在一些 STL 实现中使用了表达式模板

### [群友提交](src/群友提交/第15题)

### 标准答案
