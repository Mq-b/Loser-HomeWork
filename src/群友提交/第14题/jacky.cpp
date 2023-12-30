#include <iostream>

#define ss

namespace ss {
    int a = 0;
}

int main()
{
    a = 100;
    std::cout << ss::a << '\n';
}
