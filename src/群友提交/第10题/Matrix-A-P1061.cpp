#include<iostream>
#include<cstdint>

template<typename T>
    requires std::is_aggregate_v<T>
std::size_t size(const T& t) {
    const auto& [...elems] = t;
    return sizeof...(elems);
}

template<typename T, typename Func>
    requires std::is_aggregate_v<T>
void for_each_member(const T& t, Func func) {
    const auto& [...elems] = t;
    (func(elems), ...);
}

int main() {
    std::string ts{ "123" };
    struct X { const std::string& s; } x { ts };
    struct Y { double a{}, b{}, c{}, d{}; }y;
    std::cout << size(x) << std::endl;
    std::cout << size(y) << std::endl;

    auto print = [](const auto& member) {
        std::cout << member << ' ';
        };
    for_each_member(x, print);
    std::cout << std::endl;
    for_each_member(y, print);
    std::cout << std::endl;
}
