#include <iostream>
#include <type_traits>

struct Anyth {
	template <typename T>
	operator T(); 
};

template<typename T,typename... Args>
consteval size_t size(Args&&... args)
{
	static_assert(std::is_aggregate_v<T>);
	if constexpr (!requires{T{ args... }; })
	{
		return sizeof...(args) - 1;
	}
	else
	{
		return size<T>(args..., Anyth{});	
	}
}

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