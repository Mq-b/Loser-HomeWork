#include <format>
#include <iostream>

struct Functor {
	explicit Functor(const char*str):_str(str){}
	auto operator()(const auto&...args){
		return std::vformat(this->_str,std::make_format_args(args...));
	}
private:
	const char* _str;
};

auto operator ""_f(const char*str,size_t len){
	return Functor{str};
}

int main(){
	std::cout << "乐 :{} *\n"_f(5);
    std::cout << "乐 :{0} {0} *\n"_f(5);
    std::cout << "乐 :{:b} *\n"_f(0b01010101);
    std::cout << "{:*<10}"_f("卢瑟");

    std::cout << '\n';
    int n{};
    std::cin >> n;
    std::cout << "π：{:.{}f}\n"_f(std::numbers::pi_v<double>, n);
}
