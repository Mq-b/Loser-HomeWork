#include <functional>
#include <iostream>
#include <vector>

template <typename T, typename Func> 
auto operator | (T v1, Func f)
{
	for (auto & i:v1) { f (i);} 
	return v1;
}

int main()
{
	std::vector   v { 1, 2, 3};
	std::function f { [](const int &i) { std::cout << i << ' '; } };
	auto f2 = [](int &i) { i *= i; };
	v | f2 | f;
}
