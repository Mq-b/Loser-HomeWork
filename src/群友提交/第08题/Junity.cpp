#include<iostream>

template<class Ty,size_t size>
struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};

template<typename T,typename... T2>
struct array_info{
    using type = T;
    const static size_t size = array_info<T2...>::size + 1;
};

template<typename T>
struct array_info<T>{
    using type = T;
    const static size_t size = 1;
};

template<typename... TArgs>
array(TArgs... args) -> array<typename array_info<TArgs...>::type,array_info<TArgs...>::size>;

int main() {
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr) {
        std::cout << i << ' ';
    }
}
