//!msvc
#include<iostream>

extern "C" {
    namespace ss {
        int a = 0;
    }
}

extern "C" int a;

int main() {
    a = 100;
    std::cout << ss::a << '\n';
}
