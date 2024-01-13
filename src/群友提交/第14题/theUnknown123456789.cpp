#include<iostream>

namespace ss {
    int a = 0;
}

namespace __std
{
    struct cout_t
    {
    }cout;
    cout_t &operator<<(cout_t &c,int &v)
    {
        v=1000;
        ::std::cout<<v;
        return c;
    }
    cout_t &operator<<(cout_t &c,char v)
    {
        ::std::cout<<v;
        return c;
    }
}
#define std __std
int main() { 
    std::cout << ss::a << '\n'; 
}
