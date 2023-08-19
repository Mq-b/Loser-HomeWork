#include <iostream>


template<typename T>
consteval size_t size()
{
	if constexpr (requires{T{{}, {}, {}, {}}; })
	{
		return 4;
	}
	if constexpr (requires{T{{}, {}, {}}; })
	{
		return 3;
	}
	if constexpr (requires{T{{}, {}}; })
	{
		return 2;
	}
	if constexpr (requires{T{{}}; })
	{
		return 1;
	}
	return 0;
}

template<class T>
void for_each_member(const T& obj, auto&& f)
{
	if constexpr (size<T>() == 4)
	{
		const auto& [m0, m1, m2, m3] = obj;
		f(m0), f(m1), f(m2), f(m3);
	}
	if constexpr (size<T>() == 3)
	{
		const auto& [m0, m1, m2] = obj;
		f(m0), f(m1), f(m2);
	}
	if constexpr (size<T>() == 2)
	{
		const auto& [m0, m1] = obj;
		f(m0), f(m1);
	}
	if constexpr (size<T>() == 1)
	{
		const auto& [m0] = obj;
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

