//!msvc
#include<iostream>
#include<atomic>

namespace ss {
    int a = 0;
}

int main() {
    extern int b asm("_ZN2ss1aE");
    b = 100;
    std::atomic_thread_fence(std::memory_order_acq_rel);
    std::cout << ss::a << '\n'; 
}
