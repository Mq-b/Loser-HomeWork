#include <iostream>
#include <type_traits>

template<typename T, std::size_t N>
struct array {    
    T* begin() {
        return arr;
    }
    T* end() {
        return arr + N;
    }
    
    T arr[N];
};

template<typename T,typename... Ts>
concept same_all = (std::is_same_v<T,Ts> && ...);

template<typename T,typename... Ts>
    requires same_all<T,Ts...>
array(T,Ts...) -> array<T, sizeof...(Ts) + 1>;

int main() 
{
    ::array arr{1,2,3,4,5};

    for (const auto& v : arr) {
        std::cout << v << ' ';
    }

    return 0;
}