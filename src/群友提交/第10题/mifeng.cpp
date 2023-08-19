// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

template <size_t s>
struct tag : tag<s - 1> {};

template <>
struct tag<0> {};

struct init
{
	template <class T>
	operator T();
};

template <class T>
constexpr auto size(tag<4>) -> decltype(T{ init{}, init{}, init{}, init{} }, 0u)
{
	return 4u;
}

template <class T>
constexpr auto size(tag<3>) -> decltype(T{ init{}, init{}, init{} }, 0u)
{
	return 3u;
}

template <class T>
constexpr auto size(tag<2>) -> decltype(T{ init{}, init{} }, 0u)
{
	return 2u;
}

template <class T>
constexpr auto size(tag<1>) -> decltype(T{ init{} }, 0u)
{
	return 1u;
}

template <class T>
	requires std::is_aggregate_v<T>
constexpr size_t size() 
{
	return size<T>(tag<4>{});
}

template <class T, class F>
	requires std::is_aggregate_v<T>
constexpr void for_each_member(const T& obj, F func) 
{
	if constexpr (size<T>() == 4)
	{
		const auto& [m0, m1, m2, m3] = obj;
		func(m0); func(m1); func(m2); func(m3);
	}
	else if constexpr (size<T>() == 3) 
	{
		const auto& [m0, m1, m2] = obj;
		func(m0); func(m1); func(m2);
	}
	else if constexpr (size<T>() == 2)
	{
		const auto& [m0, m1] = obj;
		func(m0); func(m1); 
	}
	else if constexpr (size<T>() == 1)
	{
		const auto& [m0] = obj;
		func(m0); 
	}
}

int main() 
{
	struct X { std::string s{ " " }; }x;
	struct Y { double a{}, b{}, c{}, d{}; }y;

	std::cout << size<X>() << '\n';
	std::cout << size<Y>() << '\n';

	auto print = [](const auto& member) 
	{
		std::cout << member << ' ';
	};

	for_each_member(x, print);
	for_each_member(y, print);
}