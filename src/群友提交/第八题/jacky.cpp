#include <iostream>
#include <concepts>

template<class Ty,size_t size>
struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};

template <class A, class... B>
    requires std::conjunction_v<std::is_same<A, B>...>
array(A, B...) -> array<A, sizeof...(B) + 1>;

int main() {
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr) {
        std::cout << i << ' ';
    }
}
