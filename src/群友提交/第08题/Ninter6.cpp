#include <iostream>
template<class Ty,size_t size>
struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};

template <class... Args>
array(Args...) -> array<std::common_type_t<Args...>, sizeof...(Args)>;

int main() {
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr) {
        std::cout << i << ' ';
    }
}
