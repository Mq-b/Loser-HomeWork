#include<iostream>
#include<cstdint>

namespace ss {
    int a = 0;
}

#if defined(_MSC_VER)
#define MAIN_ATTRIBUTES __pragma(optimize("", off))
#elif defined(__clang__)
#define MAIN_ATTRIBUTES [[clang::optnone]]
#else
#define MAIN_ATTRIBUTES [[gnu::optimize(0)]]
#endif

int b = 0;
MAIN_ATTRIBUTES
int main() {
    *(int*)((uint8_t*)&b - sizeof(int)) = 100;
    *(int*)((uint8_t*)&b + sizeof(int)) = 100;
    std::cout << ss::a << '\n';
}
