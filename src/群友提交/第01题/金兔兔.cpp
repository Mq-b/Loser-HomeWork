import std;

template<typename T>
struct Lazy
{
	using FnType = std::function<void(T&)>;
	std::vector<T> v;
	mutable FnType map;

	Lazy(std::vector<T> v) noexcept :v(v), map([](T& x) {}) {}
	Lazy(std::vector<T> v, FnType map) noexcept :v(v), map(map) {}
	~Lazy() noexcept
	{
		for (auto& x : v)
			map(x);
	}
	const Lazy& operator|(FnType map2) const noexcept
	{
		auto old = map;
		map = [map2, old](T& x) {
			old(x);
			map2(x);
			};
		return *this;
	}
};

template<typename T, typename F>
Lazy<T> operator|(const std::vector<T>& c, const F& map) noexcept
{
	return Lazy{ c } | std::function<void(T&)>(map);
}

int main()
{
	std::vector v{ 1, 2,3 };
	std::function f{ [](const int& i) {std::cout << i << ' '; } };
	auto f2 = [](int& i) {i *= i; };
	v | f2 | f;
}