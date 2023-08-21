#include <iostream>
#include <tuple>

using namespace std;

// is braces constructible
template <typename T, typename... Args>
struct is_braces_constructible {
private:
	template <typename U, typename... UArgs>
	static auto test(int) -> decltype(U{ std::declval<UArgs>()... }, std::true_type());
	template <typename, typename...>
	static auto test(...) -> std::false_type;
public:
	static constexpr bool value = decltype(test<T, Args...>(0))::value;
};

// anytype
struct any_type {
	template <typename T>
	constexpr operator T() const;
};

// struct to tuple
template<class T>
auto to_tuple(T&& object) noexcept {
	using type = std::decay_t<T>;
	if constexpr (is_braces_constructible<type, any_type, any_type, any_type, any_type>::value) {
		auto&& [p1, p2, p3, p4] = object;
		return std::make_tuple(p1, p2, p3, p4);
	}
	else if constexpr (is_braces_constructible<type, any_type, any_type, any_type>::value) {
		auto&& [p1, p2, p3] = object;
		return std::make_tuple(p1, p2, p3);
	}
	else if constexpr (is_braces_constructible<type, any_type, any_type>::value) {
		auto&& [p1, p2] = object;
		return std::make_tuple(p1, p2);
	}
	else if constexpr (is_braces_constructible<type, any_type>::value) {
		auto&& [p1] = object;
		return std::make_tuple(p1);
	}
	else {
		return std::make_tuple();
	}
}


// get tuple size
template <typename T>
constexpr auto size() {
	return std::tuple_size_v<decltype(to_tuple(std::declval<T>()))>;
}

// for each element in tuple
template <typename T, typename F>
void for_each_member(T const& object, F&& f) {
	std::apply([&f](auto const&... args) {
		(f(args), ...);
	}, to_tuple(object));
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
