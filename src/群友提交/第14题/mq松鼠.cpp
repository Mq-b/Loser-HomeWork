#include<iostream>

namespace ss {
    int a = 0;
}
int main() {

    using namespace ss;
    a=100;
    std::cout << ss::a << '\n';
}