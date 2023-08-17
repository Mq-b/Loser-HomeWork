#include <concepts>
#include <iostream>
#include <type_traits>

template <class Ty, size_t size>
struct array
{
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};

// 参考clang里面的实现 学到了两个东西

// 1 模板类型推导指引 (Template Argument Deduction for Class Templates)
// 这里 arr{1,2,3,4,5} 被推导为array<int, 5> arr{1,2,3,4,5};
// T为类型 sizeof...(U) + 1为大小 +1 为了被吃掉的第一个参数

// 2 为什么不写构造函数也能编译
// C++17 编译器对聚合初始化进行了增强 这里因为能够推导成功，聚合初始化直接给arr的每个元素赋值

template <class T, class... U>
concept same_type = (std::is_same_v<T, U> && ...);

template <class T, class... U>
    requires same_type<T, U...>
array(T, U...) -> array<T, 1 + sizeof...(U)>;

int main()
{
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr)
    {
        std::cout << i << ' ';
    }
}
