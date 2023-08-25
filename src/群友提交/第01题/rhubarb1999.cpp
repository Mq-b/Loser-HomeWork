#include <vector>
#include <iostream>
#include <functional>

template <typename U, typename F>
std::vector<U> &operator|(std::vector<U> &v, F f)
{
    for (auto &i : v)
    {
        f(i);
    }
    return v;
}
int main()
{
    std::vector v{1, 2, 3};
    std::function f{[](const int &i)
                    { std::cout << i << ' '; }};
    auto f2 = [](int &i)
    { i *= i; };
    v | f2 | f;
}