#include<iostream>

namespace ss {
    int a = 0;
}

int main() {
    extern int _ZN2ss1aE;
#ifdef _MSC_VER
#pragma comment(linker, "/alternatename:?_ZN2ss1aE@@3HA=?a@ss@@3HA")
#endif
    _ZN2ss1aE = 100;
    std::cout << ss::a << '\n';
}
