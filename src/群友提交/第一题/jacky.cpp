#include <iostream>
#include <vector>
#include <functional>
#include <utility>

template<class Object, class Operator>
auto&& operator|(Object&& obj, Operator opt) {
	for(auto&& elon : std::forward<Object>(obj)){
		opt(std::forward<decltype(elon)>(elon));
	}
	return std::forward<Object>(obj);
}

int main() {
    std::vector v{1, 2, 3};
    std::function f{[](const int& i) { std::cout << i << ' '; }};
    auto f2 = [](int& i) { i *= i; };
    v | f2 | f;
}
