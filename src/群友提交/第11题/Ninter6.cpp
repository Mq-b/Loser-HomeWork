#include <vector>

struct Pos
{
    int x;
    int y;
};

int main()
{
    std::vector<Pos> vec;
    vec.emplace_back(1, 5);
}

/* 
这个涉及了 apple clang 的一个非常经典的问题
emplace_bace内部调用了std::construct_at这个函数
而这个函数实现是这样的
```c++
template<class T, class... Args>
constexpr T* construct_at( T* p, Args&&... args ) {
    return ::new (const_cast<void*>(static_cast<const volatile void*>(p)))
        T(std::forward<Args>(args)...);
}
```
实际上是括号初始化了这个类型实例
对于聚合体，括号初始化是非法的，但c++20之后允许了这种行为

而 apple clang 这个逆天玩意说自己支持c++2b，实际上连c++17都支持不全，三天两头报这个错
而且一报错就一堆，全是报什么标准库头文件里面的问题，根本找不到哪里有问题
*/