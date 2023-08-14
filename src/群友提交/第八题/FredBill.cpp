#include <initializer_list>
#include <iostream>

template <class Ty, size_t size> struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};

template <class T, class... U>
    requires(std::is_same_v<T, U> && ...)
array(T, U...) -> array<T, 1 + sizeof...(U)>;

int main() {
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr) { std::cout << i << ' '; }
}