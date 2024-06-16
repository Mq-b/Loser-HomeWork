#include<iostream>
#include<vector>
#include<functional>

using namespace std;

static auto& operator|(vector<int>& in, auto f) {
    for (auto& it : in) {
        f(it);
    }
    return in;
}

int main() {
    std::vector v{ 1, 2, 3 };
    std::function f{ [](const int& i) {std::cout << i << ' '; } };
    auto f2 = [](int& i) {i *= i; };
    v | f2 | f;
}
