#include <iostream>
#include <utility>
namespace {
	struct _any {
		//为了更好看的扩展ints的语法
		explicit _any(auto);

		//convert to any
		template<class T>
		operator T();
	};

	template<typename T, size_t... ints>
	constexpr bool _can_size(std::integer_sequence<size_t, ints...> int_seq)
	{
		return (requires{
			T{ _any(ints)... };
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
	template<typename T>
	consteval size_t size()
	{
		return _size<T, 100>::value;
	}
}

//为了能递归使用LOOP
//https://stackoverflow.com/a/12540675
# define EMPTY(...)
# define DEFER(...) __VA_ARGS__ EMPTY()
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
# define EXPAND(...) __VA_ARGS__

#define LOOP0(body,sep)
#define LOOP1(body,sep) body(1)
#define LOOP2(body,sep) body(2) sep() LOOP1(body,sep) 
#define LOOP3(body,sep) body(3) sep() LOOP2(body,sep) 
#define LOOP4(body,sep) body(4) sep() LOOP3(body,sep) 


//从n到1的重复循环
//LOOP(n) EXPAND TO: body(n) sep() body(n-1) ... body(1)
#define LOOP(n,body,sep) LOOP ## n(body,sep)
#define LOOP_ID() LOOP

template<class T>
void for_each_member(const T& obj, auto&& f)
{
	//#define EMPTY()
#define COMMA() ,

	//m0
#define MAKE_M_VAR(n) m##n
//f(m0)
#define MAKE_F_CALL(n) f(m##n)
//const auto& [m0, m1, m2, m3] = obj;
#define MAKE_UNPACK(n) const auto& [ DEFER(LOOP_ID)()(n,MAKE_M_VAR,COMMA) ] = obj
//f(m0), f(m1), f(m2), f(m3);
#define MAKE_CALL(n) DEFER(LOOP_ID)()(n, MAKE_F_CALL, COMMA);

//MAKE_ALL_CHECK(4) EXPAND TO:
//if constexpr (size<T>() == 4)
//{
//	const auto& [m0, m1, m2, m3] = obj;
//	f(m0), f(m1), f(m2), f(m3);
//}
#define MAKE_ALL_CHECK(n) \
	if constexpr (size<T>() == n)\
	{\
		MAKE_UNPACK(n);\
		MAKE_CALL(n);\
	}

	//这里LOOP被递归调用了
	
	/*
	expand to:

	if constexpr (size<T>() == 4) {
		const auto& [m4, m3, m2, m1] = obj; f(m4), f(m3), f(m2), f(m1);;
	} if constexpr (size<T>() == 3) {
		const auto& [m3, m2, m1] = obj; f(m3), f(m2), f(m1);;
	} if constexpr (size<T>() == 2) {
		const auto& [m2, m1] = obj; f(m2), f(m1);;
	} if constexpr (size<T>() == 1) {
		const auto& [m1] = obj; f(m1);;
	};
	*/
	EXPAND(LOOP(4, MAKE_ALL_CHECK, EMPTY));




#undef MAKE_ALL_CHECK
#undef MAKE_CALL
#undef MAKE_UNPACK
#undef MAKE_F_CALL
#undef MAKE_M_VAR
#undef COMMA
#undef EMPTY
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
