#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

std::vector<int>& operator|(std::vector<int>& v,
                            const std::function<void(int&)>& f) {
  std::for_each(std::begin(v), std::end(v), f);
  return v;
}

int main() {
  std::vector v{1, 2, 3};
  std::function f{[](const int& i) { std::cout << i << ' '; }};
  auto f2 = [](int& i) { i *= i; };
  v | f2 | f;
}