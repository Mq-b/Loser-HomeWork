#include <iostream>
template<class Ty, size_t size>
struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};

template<typename Ty, typename...Args>
    requires (std::is_same_v<Ty, Args>&&...)
array(Ty, Args...)->array<Ty, sizeof...(Args) + 1>;

int main() {
    ::array arr{ 1, 2, 3, 4, 5 };
    for (const auto& i : arr) {
        std::cout << i << ' ';
    }
}