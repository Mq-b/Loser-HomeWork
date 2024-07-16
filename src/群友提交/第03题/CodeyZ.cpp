#include<iostream>
#include<format>
#include<string.h>
#include<string_view>
struct Frac {
	int a, b;
};

template<>
struct std::formatter<Frac, char> {

	template<class ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		auto it = ctx.begin();
		if (it == ctx.end())
			return it;
		if (it != ctx.end() && *it != '}')
			throw std::format_error("无效的 QuotableString 格式参数。");
		return it;
	}
	template<class Fmtcontext>
	auto format(::Frac f, Fmtcontext& ctx)const {
		return format_to(ctx.out(), "{}/{}", f.a, f.b);
	}
};

template<typename...Args>
auto print(std::string_view s,Args&&...args) {
	std::cout << std::vformat(s,std::make_format_args(args...)) << '\n';
}

int main() {
	Frac f{ 1,10 };
	print("{}", f);// 结果为1/10
	//std::cout << std::format("{}", f);
}