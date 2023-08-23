#include <iostream>
#include <format>
struct MyStruct
{
	std::string m_s;
	MyStruct(std::string&& s) :m_s(s) {};
	auto operator()(auto value)
	{
		return std::vformat(m_s, std::make_format_args(value));
	}
};
MyStruct operator ""_f(const char* s,size_t x) 
{
	return MyStruct(s);
}
int main() 
{
	std::cout << "乐 :{} *\n"_f(5);
	std::cout << "乐 :{0} {0} *\n"_f(5);
	std::cout << "乐 :{:b} *\n"_f(0b01010101);
	std::cout << "{:*<10}"_f("卢瑟");
	std::cout << '\n';
}
