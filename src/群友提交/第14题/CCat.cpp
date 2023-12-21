#include<iostream>

namespace ss {
    int a = 0;
}

namespace ss {
    extern int b;
}

int ss::b = a = 100;

int main() {
    std::cout << ss::a <<'\n';
}
