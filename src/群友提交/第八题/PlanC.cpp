#include <iostream>
template<class Ty, size_t size>
struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};

template<typename Ty, typename ...Args>
array(Ty, Args...) -> array<std::enable_if_t<(std::is_same_v<Ty, Args>&&...), Ty>, sizeof...(Args) + 1>;

int main() {
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr) {
        std::cout << i << ' ';
    }
}