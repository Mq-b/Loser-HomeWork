#include<type_traits>
#include<algorithm>
#include<iostream>

//lambda包一下，done

template<class TL, class F>
struct meta_function {};

template<template<class...> class TL, class F, class ...L>
struct meta_function<TL<L...>, F>
{
	F func;
	meta_function(TL<L...> const& tl, F const& f) :func(f) {}
	auto operator()(L...l)
	{
		return func.template operator() < L... > (std::forward<L>(l)...);
	}
};

template<template<class...> class TL, class F, class ...L>
meta_function(TL<L...>const& tl, F const& f) -> meta_function<TL<L...>, F>;

template<class TL, class F>
auto realize_meta(F const& f)
{
	return meta_function{ TL(), f };
}
template<typename ...> struct tl {};

#define BY_NAME(x) []<typename T, typename ...Args>(T a, Args...args)\
{\
	return x<T>(a, args...);\
}\



template<typename F, class...Args>
auto foo(F&& f, Args&&...args) {
	return realize_meta<tl<Args...>>(f)(args...);
}



int main() {
	const auto result = foo(BY_NAME(std::min), 2, 3);
	std::cout << result << '\n';
}
