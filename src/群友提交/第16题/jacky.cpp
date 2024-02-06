#include <iostream>
#include <utility>

template <typename F, class... Args>
auto foo(F func, Args&&... args)
{
    return func(std::forward<Args>(args)...);
}

#define BY_NAME(func) \
    [](auto&&... args) noexcept(noexcept(func(std::forward<decltype(args)>(args)...))) { return (func)(std::forward<decltype(args)>(args)...); }

int main()
{
    const auto result = foo(BY_NAME(std::min), 2, 3);
    std::cout << result << '\n';
}
