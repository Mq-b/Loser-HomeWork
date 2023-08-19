第十题解答 
== 
也不算解答吧，终于看完那篇文章了，因为就算写出来也不算我写的就用md解释一下吧  
**思路:**先计算出结构体成员数量，再用C++17结构化绑定的语法把成员解包，分别应用函数  
1.先拿结构体成员数量
```c++
struct init
{                                这个结构体的转换函数能转换成任意类型
  template <typename T>
  operator T(); 
};  

template <typename T>
constexpr auto size_(tag<4>) -> decltype(T{init{}, init{}, init{}, init{}}, 0u) 
{ return 4u; }  
```
这里学会一个很有意思的写法:**一种常见手法，是在返回类型上使用表达式 SFINAE，其中表达式使用逗号运算符，其左子表达式是所检验 的（转型到 void 以确保不会选择返回类型上的用户定义逗号运算符），而右子表达式具有期望函数返回的类型。**[参见cpprefence](https://zh.cppreference.com/w/cpp/language/sfinae)
类似于函数重载，不知道这样说对不对，因为结构体可以使用花括号初始化器列表初始化，则当类型可以如`T{init{}, init{}, init{}, init{}`这样展开时对应匹配,decltype(0u)返回4，不匹配时从重载集中删除。
```c++
template <typename T>
constexpr auto size_(tag<3>) -> decltype(T{init{}, init{}, init{}}, 0u)
{ return 3u; }
 
template <typename T>
constexpr auto size_(tag<2>) -> decltype(T{init{}, init{}}, 0u)
{ return 2u; }
 
template <typename T>
constexpr auto size_(tag<1>) -> decltype(T{init{}}, 0u)
{ return 1u; }
 
template <typename T>
constexpr auto size_(tag<0>) -> decltype(T{}, 0u)
{ return 0u; }

template <unsigned I>
struct tag : tag<I - 1> {};   
template <>
struct tag<0> {}; 
```
此处又是个技巧,因为函数重载匹配选取最佳匹配，我们考虑如下继承链 A->(派生)B->C->D 与如下函数f(A),f(B),f(C),f(D),那么`D d{};f(d)`执行调用时选取f(D),如果f(D)函数不存在那么选取f(C),依次类推具。体参考cppreference[隐式转换序列的排行](https://zh.cppreference.com/w/cpp/language/overload_resolution)  
此处我们模拟一个实例`struct Y { double a{}, b{}; }y;`首先经过以下函数size_(tag<4>{})
此处tag<4>{}这个亡值表达式正如上述继承链中的D即使他的最佳匹配从重载记中剔除也能匹配包含他的父类作为形参的函数。SFINAE实例y不匹配，那么选择size_(tag<3>)的重载版本以此类推选取到合适版本size_(tag<2>),返回结果2  
而开始的`init{}`结构体正是帮助我们进行类型转换从而进行SFINAE
```c++ 
template <typename T>
constexpr size_t size() 
{ 
  static_assert(std::is_aggregate_v<T>);
  return size_<T>(tag<4>{});
}
```
2.结构化绑定解包结构体，对每个成员分别调用函数f()  
获取完结构体成员数量就可以用`if constexpr`与[c++17的结构化绑定](https://zh.cppreference.com/w/cpp/language/structured_binding)**绑定指定名称到初始化器的子对象或元素。类似引用，结构化绑定是既存对象的别名。不同于引用的是，结构化绑定的类型不必为引用类型**
```c++
template <typename T, typename F>
void for_each_member(T const& v, F&& f) {
	static_assert(std::is_aggregate_v<T>);

	if constexpr (size<T>() == 4u) {
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
```
后记
--
白老师你这题就是你那个知乎文章吧，我也看了你c++20的写法，主要区别还是获取成员数量的写法，感觉那个写法比这个打表适用性强，这tm结构题成员越多还要写更多重载。