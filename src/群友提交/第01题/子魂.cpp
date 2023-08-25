#include <functional>
#include <iostream>
#include <vector>

template<typename C, typename F>
auto operator|(C&& c, F&& f)
{
    for(auto&& elem: c)
    {
        f(elem);
    }
    return std::forward<C&&>(c);
}

int main()
{
    std::vector v{ 1, 2, 3 };
    std::function f{ [](const int& i){ std::cout << i << ' '; } };
    auto f2 = [](int& i){ i *= i; };
    v | f2 | f;
    return 0;
}
