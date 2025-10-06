#include <functional>
#include <vector>
#include <iostream>
template <typename Func>
std::vector<int> operator|(std::vector<int> v, Func func)
{
    for (auto &i : v)
    {
        func(i);
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