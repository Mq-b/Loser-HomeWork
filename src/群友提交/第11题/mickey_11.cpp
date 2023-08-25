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
    // Pos p(1, 5); 其实就是判断这个是否合法
}

// 这个问题涉及到2个地方 emplace_back 和 聚合类型的初始化
// 简单回答
// 本质在于 Pos p(1, 5) 在c++20前非法，在c++20后合法
// C++20前 Pos没有Pos(int, int)这个构造函数可供emplace_back使用
// C++20后拓展了aggregates, 允许 Parens-init for aggregates,而不仅仅是brace-init

// 详细回答
// emplace_back的实现是个万能引用，转发参数后到对应结构的构造函数
// 在c++20前，aggregates只有brace-init {}, 没有parens-init ()
// 而emplace_back需要()这样的构造函数，找不到自然就编译错
// C++20干了个事情 P0960 “Allow initializing aggregates from a parenthesized list of values,”
// 这下Pos p(1, 5)这个语法就合法了,所以emplace_back可以使用这个方法构造对象

// 参考文档
// 1. https://quuxplusone.github.io/blog/2022/06/03/aggregate-parens-init-considered-kinda-bad/
// 2. https://en.cppreference.com/w/cpp/language/aggregate_initialization#Designated_initializers
