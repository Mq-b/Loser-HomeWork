#include<type_traits>
#include<algorithm>
#include<iostream>

#define BY_NAME(x) []<typename ...Args>(Args...args)\
{\
	return x<std::common_type_t<Args...>>(args...);\
}\

template<typename F, class...Args>
auto foo(F f, Args&&...args) {
    return f(std::forward<Args>(args)...);
}

int main() {
    const auto result = foo(BY_NAME(std::min), 2, 3);
    std::cout << result << '\n';
}
