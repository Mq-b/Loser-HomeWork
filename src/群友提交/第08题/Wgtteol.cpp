#include <iostream>
#include <array>
template<class Ty, size_t size>
struct array {
	Ty* begin() { return arr; }
	Ty* end() { return arr + size; }
	Ty arr[size];
};

/*
doc:http s://www.noerror.net/zh/cpp/container/array/deduction_guides.html
*/
template<class T, class... U> array(T, U...) -> array<T, 1 + sizeof...(U)>;

int main()
{
	::array arr{1, 2, 3, 4, 5};
	for (const auto& i : arr) {
		std::cout << i << ' ';
	}
	return 0;
}