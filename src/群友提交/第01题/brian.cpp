//
// loser_homework / brian.cpp
// Created by brian on 2024 Apr 03.
//

#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>

template<typename T, typename Func>
requires std::invocable<Func, T&>
std::vector<T>& operator|(std::vector<T>& vec, Func const& func) {
  std::ranges::for_each(vec, func);
  return vec;
}


int main() {
  std::vector v{1, 2, 3};
  std::function f{[](const int& i) { std::cout << i << ' '; }};
  auto f2 = [](int& i) { i *= i; };
  v | f2 | f;
}