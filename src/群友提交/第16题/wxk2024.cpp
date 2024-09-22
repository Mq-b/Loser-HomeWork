#include <iostream>

template <typename F, class... Args> auto foo(F f, Args &&...args) {
  return f(std::forward<Args>(args)...);
}

#define BY_NAME(func)                                                          \
  ([](auto &&...args) -> decltype(auto) {                                      \
    return func(std::forward<decltype(args)>(args)...);                        \
  })

int main() {
  const auto result = foo(BY_NAME(std::min), 2, 3);
  std::cout << result << '\n';
}
