//
// loser_homework / scope_guard.cpp
// Created by brian on 2024 Apr 03.
//
#include <cstdio>
#include <cassert>

#include <stdexcept>
#include <functional>

struct X {
  X() { puts("X()"); }

  X(const X&) { puts("X(const X&)"); }

  X(X&&) noexcept { puts("X(X&&)"); }

  ~X() { puts("~X()"); }
};

struct scope_guard {
  template<typename F, typename... A>
  requires std::invocable<F, A...>
  scope_guard(F&& func, A&& ...args) {
    gc = [&func, &args...]() mutable {
      /**
      * @brief https://en.cppreference.com/w/cpp/utility/functional/invoke
      * invoke： 如果\p func 是成员函数， 则\p ...args 的第一个参数是对应的对象(\p &,* )
      */
      std::invoke(std::forward<F>(func), std::forward<A>(args)...);
    };
  }
  ~scope_guard() { gc(); }
  scope_guard(const scope_guard&) = delete;
  scope_guard& operator=(const scope_guard&) = delete;

private:
  std::function<void()> gc;
};


int main() {
  {
    // scope_guard的作用之一，是让各种C风格指针接口作为局部变量时也能得到RAII支持
    // 这也是本题的基础要求
    FILE* fp = nullptr;
    try {
      fp = fopen("test.txt", "a");
      auto guard = scope_guard([&] {
        fclose(fp);
        fp = nullptr;
      });

      throw std::runtime_error{"Test"};
    } catch (std::exception& e) {
      puts(e.what());
    }
    assert(fp == nullptr);
  }
  puts("----------");
  {
    // 附加要求1，支持函数对象调用
    struct Test {
      void operator()(X* x) {
        delete x;
      }
    } t;
    auto x = new X{};
    auto guard = scope_guard(t, x);
  }
  puts("----------");
  {
    // 附加要求2，支持成员函数和std::ref
    auto x = new X{};
    {
      struct Test {
        void f(X*& px) {
          delete px;
          px = nullptr;
        }
      } t;
      auto guard = scope_guard{&Test::f, &t, std::ref(x)};
    }
    assert(x == nullptr);
  }
}