#include <iostream>
#include <utility>
namespace {
	struct _init {
		template<class T>
		operator T();
	};

	template<typename T, size_t... ints>
	constexpr bool _can_size(std::integer_sequence<size_t, ints...> int_seq)
	{
		return (requires{
			T{(ints, _init{})...};
		});
	}

	//T{N个{}} 可以吗
	template<typename T, size_t N>
	constexpr bool can_size = _can_size<T>(std::make_integer_sequence<size_t, N>());

	//T{N个{}} 正好可以吗
	template<typename T, size_t N>
	constexpr bool is_excat_size = can_size<T, N> && !can_size<T, N + 1>;


	//从大到小循环到正好is_excat_size<T,n>是true
	template<typename T, size_t N>
	struct _size;

	template<typename T>
	struct _size<T, 0>
	{
		static constexpr size_t value = 0;
	};

	template<typename T, size_t N>
	struct _size
	{
		static constexpr size_t value = is_excat_size<T, N> ? N : _size<T, N - 1>::value;
	};
}




template<typename T>
consteval size_t size()
{
	return _size<T,100>::value;
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

