#include<iostream>
#include<cstdint>
#include<atomic>

namespace ss {
    int a = 0;
}
int b = 0;
int main() {
    *const_cast<volatile int*>(&b - 1) = 100;
    *const_cast<volatile int*>(&b + 1) = 100;
    std::atomic_flag{}.test_and_set();
    std::cout << ss::a << '\n';
}
