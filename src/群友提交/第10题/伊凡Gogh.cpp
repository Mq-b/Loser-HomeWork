#include <iostream>

template <class T>
concept aggregate = std::is_aggregate_v<std::remove_cvref_t<T>>;

struct placeholder {
	template <class T>
	operator T() const
	{
		return T{};
	}
};

template <aggregate T, placeholder... _Members>
consteval size_t size()
{
	if constexpr (requires { T{ _Members... }; })
		return size<T, _Members..., placeholder{}>();
	else
		return sizeof...(_Members) - 1;
}

void for_each_member(aggregate auto const &object, [[maybe_unused]] auto &&print_function)
{
	using T = std::remove_cvref_t<decltype(object)>;
	if constexpr (size<T>() == 1)
	{
		const auto &[member1] = object;
		print_function(member1);
	}
	else if constexpr (size<T>() == 2)
	{
		const auto &[member1, member2] = object;
		print_function(member1);
		print_function(member2);
	}
	else if constexpr (size<T>() == 3)
	{
		const auto &[member1, member2, member3] = object;
		print_function(member1);
		print_function(member2);
		print_function(member3);
	}
	else if constexpr (size<T>() == 4)
	{
		const auto &[member1, member2, member3, member4] = object;
		print_function(member1);
		print_function(member2);
		print_function(member3);
		print_function(member4);
	}
}

int main()
{
	struct A {
	} a;
	struct B {
		std::string s{ "abcdef" };
	} b;
	struct C {
		unsigned	i{ 123456 };
		const char *s = "ABCDEF";
	} c;
	struct D {
		long long	i{ 123456123456 };
		std::string s{ "!@#$%^" };
		double		d{ 1.234e-56 };
	} d;
	struct E {
		double a{ 1.2 }, b{ 3.4 }, c{ 5.6 }, d{ 7.8 };
	} e;
	struct F {
		int a{ 1 }, b{ 2 }, c{ 3 }, d{ 4 }, e{ 5 };
	} f;
	std::cout << size<A>() << std::endl;
	std::cout << size<B>() << std::endl;
	std::cout << size<C>() << std::endl;
	std::cout << size<D>() << std::endl;
	std::cout << size<E>() << std::endl;
	std::cout << size<F>() << std::endl;

	auto print = [](const auto &member) { std::cout << member << ' '; };
	for_each_member(a, print);
	for_each_member(b, print);
	for_each_member(c, print);
	for_each_member(d, print);
	for_each_member(e, print);
	for_each_member(f, print);
	return 0;
}
