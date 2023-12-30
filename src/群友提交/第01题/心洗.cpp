import std;

template<typename C, typename F>
auto operator|(C&& container, F&& func)
{
	for (auto&& item : container)
		func(item);

	return container;
}
int main()
{
	std::vector v{ 1, 2,3 };
	std::function f{ [](const int& i) {std::cout << i << ' '; } };
	auto f2 = [](int& i) {i *= i; };
	v | f2 | f;
}