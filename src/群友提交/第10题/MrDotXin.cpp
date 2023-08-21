#include <iostream>
#include <string>
/**
  * 学习了 https://akrzemi1.wordpress.com/2020/10/01/reflection-for-aggregates/的内容
  * 大概了解了 如何利用 SFINAE 和 重载函数对于多重继承对象的匹配规则避免调用函数不明确的错误
  * 
  * 
*/

//< 对于多重继承对象, 编译器会优先匹配相应类型, 找不到就一直向下匹配匹配基类, 直到找到匹配的函数便不会再找
/**
 * tag<> 对象作用是为size_函数的匹配添加上匹配"优先级", 
 * 对于任意的成员对象, 在初始化聚合对象T时,
 * 无论初始化多少个对象 T都满足要求, 所以需要一种方法使得
 * 在多个重载的模板函数对任意聚合类型T进行匹配时, 再找到第一
 * 个匹配的对象时就停止查找, 找不到接着查找, 这很像对于多重
 * 继承对象的匹配规则, 所以可以想到利用模板类tag<>实现
 * 
 *    
 */

template<std::size_t I>
struct tag : tag<I - 1> {};
template<>
struct tag<0>{};

/**
 * Init<> 对象类似于std::declval<> 用于在decltype中声明一个对象。
 * 这种方式的声明适用于那些不是default constributible但对于decltype
 * 的推断有必要的类型, 但是由于并不知道聚合对象结构内的对象类型, 所以
 * std::declval<>不适用, 则利用Init<>辅助模板让编译器自动推断。
 * 
 * 
*/
struct Init
{
    template<typename T>
    operator T();
};



template <typename T>
constexpr auto size_(tag<4>) 
  -> decltype(T{Init{}, Init{}, Init{}, Init{}}, 0u)
{ return 4u; }
 
template <typename T>
constexpr auto size_(tag<3>) 
  -> decltype(T{Init{}, Init{}, Init{}}, 0u)
{ return 3u; }
 
template <typename T>
constexpr auto size_(tag<2>) 
  -> decltype(T{Init{}, Init{}}, 0u)
{ return 2u; }
 
template <typename T>
constexpr auto size_(tag<1>) 
  -> decltype(T{Init{}}, 0u)
{ return 1u; }
 
template <typename T>
constexpr auto size_(tag<0>) 
  -> decltype(T{}, 0u)
{ return 0u; }
 
template <typename T>
constexpr size_t size() 
{ 
  static_assert(std::is_aggregate_v<T>);
  return size_<T>(tag<4>{});
}


template<typename T, typename Pred>
void for_each_member(const T& t, Pred p) 
{
    constexpr int s = size<T>();
    if constexpr (s == 1) 
    {
        const auto& [v1] = t;
        p(v1);
    }

    if constexpr (s == 2)
    {
        const auto& [v1, v2] = t;
        p(v1); p(v2);
    }

    if constexpr (s == 3)
    {
        const auto& [v1, v2, v3] = t;
        p(v1); p(v2); p(v3);
    }

    if constexpr (s == 4)
    {
        const auto& [v1, v2, v3, v4] = t;
        p(v1); p(v2); p(v3); p(v4);
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

    return 0;
}
