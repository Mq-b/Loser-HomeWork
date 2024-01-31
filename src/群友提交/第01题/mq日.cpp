#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>
template<typename T,typename IT>
T& operator |(T&& v,const IT& f) 
{
    for(auto&& x:v)
    {
        f(x);
    }
    return v;
}

int main()
{
    std::vector v{1, 2, 3};
    std::function f{[](const int &i){ std::cout << i << ' '; }};
    auto f2 = [](int &i){ i *= i; };
    v | f2 | f;
}