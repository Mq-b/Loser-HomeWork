import std;

struct init {
    template <typename T>
    operator T();
};

template <size_t I>
struct tag : tag<I - 1> { };

template <>
struct tag<0> { };

template <typename T>
    requires requires { T { init {}, init {}, init {}, init {} }; }
constexpr auto size_(tag<4>)
{
    return 4u;
}

template <typename T>
    requires requires { T { init {}, init {}, init {} }; }
constexpr auto size_(tag<3>)
{
    return 3u;
}

template <typename T>
    requires requires { T { init {}, init {} }; }
constexpr auto size_(tag<2>)
{
    return 2u;
}

template <typename T>
    requires requires { T { init {} }; }
constexpr auto size_(tag<1>)
{
    return 1u;
}

template <typename T>
    requires requires { T {}; }
constexpr auto size_(tag<0>)
{
    return 0u;
}

template <typename T>
constexpr size_t size()
{
    static_assert(std::is_aggregate_v<T>);
    return size_<T>(tag<4> {}); // highest supported number
}

template <typename T, typename F>
void for_each_member(T const& v, F f)
{
    static_assert(std::is_aggregate_v<T>);

    if constexpr (size<T>() == 4u) {
        const auto& [m0, m1, m2, m3] = v;
        f(m0);
        f(m1);
        f(m2);
        f(m3);
    } else if constexpr (size<T>() == 3u) {
        const auto& [m0, m1, m2] = v;
        f(m0);
        f(m1);
        f(m2);
    } else if constexpr (size<T>() == 2u) {
        const auto& [m0, m1] = v;
        f(m0);
        f(m1);
    } else if constexpr (size<T>() == 1u) {
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