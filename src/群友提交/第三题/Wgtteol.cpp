#include <format>
#include <iostream>

struct Frac {
	int a, b;
};

template<>
struct std::formatter<Frac>: std::formatter<char>  {
	/*
	auto parse(const auto& ctx) const { // fist call
		return ctx.begin();
	}
	*/

	auto format(const Frac& frac, auto& ctx) const { // second call
		return std::format_to(ctx.out(), "{}/{}", frac.a, frac.b);
	}
};

auto operator ""_f(const char* str_c, size_t n)
{
	return [=](auto&& ...elems)
	{
		return std::vformat(str_c, std::make_format_args(elems...));
	};
}

void print(std::string_view fmt, auto&&...arg) {
	std::cerr << std::vformat(fmt, std::make_format_args(arg...)) << std::endl;
}

int main() 
{
	Frac frac{ 1, 10 };
	print("{}", frac);
}