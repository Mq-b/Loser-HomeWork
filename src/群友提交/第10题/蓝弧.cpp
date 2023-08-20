//https://godbolt.org/z/3GjjW4cjT
#include<tuple>
#include<iostream>
#include<cstddef>
#include<array>
#include<cstring>
#include<cstdlib>
#include<functional>
#include<memory>
#include<boost/pfr.hpp>

#define printl($) void(std::cout<<($)<<std::endl)
using std::size_t;
void func(void* a0,void* a1,void*tp)noexcept
{
    *(&tp+5)=*(&a0+2);
    *(&tp+4)=*(&a0-2);
}
struct invoke_t
{
    void*a0;
    void*a1;
    void*tp;
    void init()noexcept
    {
        func(a0,a1,tp);
    }
};
struct invoke:invoke_t
{
    invoke(invoke_t invoke_v):invoke_t(invoke_v)
    {
        init();
    }
    void call()
    {
        std::bind(reinterpret_cast<void(*)(void*,void*)>(tp),a0,a1);
    }
    void operator()()noexcept
    {
        call();
    }
};

template<typename T,typename F>
void* get_member_function_address(F T::*P)
{
    return reinterpret_cast<void*&>(P);
}
inline constexpr std::byte operator ""_b(unsigned long long int i)noexcept
{
    return static_cast<std::byte>(i);
}
struct main_t
{
    main_t()
    {
        std::array jump_ba
        {
            0x55_b,
            0x48_b,0x89_b,0xE5_b,
            0x48_b,0x89_b,0x7D_b,0xF8_b,
            0x48_b,0x89_b,0x75_b,0xF0_b,
            0x48_b,0x89_b,0x55_b,0xE8_b,
            0x48_b,0x8D_b,0x45_b,0xE8_b,
            0x48_b,0x83_b,0xC0_b,0x28_b,
            0x48_b,0x8B_b,0x55_b,0x08_b,
            0x48_b,0x89_b,0x10_b,
            0x48_b,0x8D_b,0x45_b,0xE8_b,
            0x48_b,0x83_b,0xC0_b,0x20_b,
            0x48_b,0x8B_b,0x55_b,0xE8_b,
            0x48_b,0x89_b,0x10_b,
            0x90_b,
            0x5D_b,
            0xC3_b,
            0x90_b
        };
        std::array invoke_ba
        {
            0x55_b,
            0x48_b,0x89_b,0xE5_b,
            0x48_b,0x83_b,0xEC_b,0x08_b,
            0x48_b,0x89_b,0x7D_b,0xF8_b,
            0x48_b,0x8B_b,0x45_b,0xF8_b,
            0x48_b,0x8B_b,0x50_b,0x10_b,
            0x48_b,0x8B_b,0x45_b,0xF8_b,
            0x48_b,0x8B_b,0x48_b,0x08_b,
            0x48_b,0x8B_b,0x45_b,0xF8_b,
            0x48_b,0x8B_b,0x00_b,
            0x48_b,0x89_b,0xCE_b,
            0x48_b,0x89_b,0xC7_b,
            0xE8_b,0xC4_b,0xDB_b,0xFF_b,0xFF_b,
            0x90_b,
            0xC9_b,
            0xC3_b,
            0x90_b
        };
        if(std::memcmp((void*)func,&jump_ba,jump_ba.size())
            or std::memcmp(get_member_function_address(&invoke_t::init),&invoke_ba,invoke_ba.size()))
        {
            printl("测试失败，该程序只能在指定的编译环境下运行。");
            std::exit(EXIT_FAILURE);
        }
    }
    ~main_t()noexcept{}
}Main_v;

template<typename T>
struct size
{
    friend std::ostream& operator<<(std::ostream&os,size)
    {
        return os<<boost::pfr::tuple_size_v<T>;
    }
};
template<typename T,typename F>
constexpr auto for_each_member(T&&v,F&&f)
{
    auto for_each_invoke_f = [&f](auto const&...v)
    {
        (invoke(invoke_t{std::make_shared<std::function<void(decltype(v) const&)>>(f).get(),(void*)&v,get_member_function_address(&std::function<void(decltype(v) const&)>::operator())})(),...);
    };
    return std::unique_ptr<void,std::function<void(void*)>>((void*)true,[&for_each_invoke_f,&v](auto){std::apply(for_each_invoke_f,boost::pfr::structure_to_tuple(v));});
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
