# Loser-HomeWork

卢瑟们的作业展示

提交`pr`不应当更改当前`README`，请将作业提交到`src\群友提交`中，比如你要提交第一个作业：

你应当在`src\群友提交\第一题`中创建一个自己的`.md`或`.cpp`文件，**文件名以QQ群名命名**。

答题的**一般要求**如下（题目额外要求也自行注意看）：

1. 不更改`main`函数，不得使其不运行（意思别捞偏门）。
2. 自行添加代码，在满足第一点的要求下，要能成功编译运行并与给出运行结果一致。

## `01`实现管道运算符
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
### 运行结果：
```
1 4 9
```

难度：**一星**

## 群友提交

答题者：[**`andyli`**](https://github.com/Mq-b/Loser-HomeWork/blob/main/src/%E7%BE%A4%E5%8F%8B%E6%8F%90%E4%BA%A4/%E7%AC%AC%E4%B8%80%E9%A2%98/andyli.cpp)
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

<br>

## 标准答案

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
不使用范围`for`，使用`C++20`简写函数模板：
```cpp
std::vector<int>& operator|(auto& v1, const auto& f) {
	std::ranges::for_each(v1, f);
	return v1;
}
```
**各种范式无非就是这些改来改去了，没必要再写。**

---

<br>

## `02`实现自定义字面量`_f`
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
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v, n);
}
```
### 运行结果:
```
乐 :5 *
乐 :5 5 *
乐 :1010101 *
卢瑟******
6
π：3.141593
```
`6`为输入，决定`π`的小数点后的位数，可自行输入更大或更小数字。
提示：`C++11用户定义字面量`，`C++20format库`。
难度：**二星**

## 群友提交

<br>

## 标准答案

```cpp
constexpr auto operator""_f(const char* fmt, size_t) {
	return[=]<typename... T>(T&&... Args) { return std::vformat(fmt, std::make_format_args(std::forward<T>(Args)...)); };
}
```

---

<br>

## `03`实现`print`以及特化`std::formatter`

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
禁止面相结果编程，使用宏等等方式，最多`B`（指评价），本作业主要考察和学习`format`库罢了。
提示: **`std::formatter`**
>提交代码最好是网上编译了三个平台的截图，如：

![图片](image/第三题/01展示.jpg)