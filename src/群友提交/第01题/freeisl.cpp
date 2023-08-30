/*********************************************************************
 * Auther : 学习令人头秃
 * Date : 2023/8/30 02:26
 *********************************************************************/

#include <iostream>
#include <vector>
#include <functional>

template <typename T1, typename T2>
auto & operator | (std::vector<T1> &lhs, T2 &rhs)
{
    for (auto &i : lhs)
    {
        rhs(i);
    }
    return lhs;
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