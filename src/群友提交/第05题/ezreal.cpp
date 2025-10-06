#include <utility>
#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <functional>

struct X
{
    X() { puts("X()"); }
    X(const X &) { puts("X(const X&)"); }
    X(X &&) noexcept { puts("X(X&&)"); }
    ~X() { puts("~X()"); }
};

struct scope_guard
{
    scope_guard(std::function<void()> func) : func_(func)
    {
    }

    template <typename Func, typename... Args>
    scope_guard(Func func, Args &&...args)
    {
        func_ = std::bind(func, std::forward<Args>(args)...);
    }

    ~scope_guard()
    {
        func_();
    }
    std::function<void()> func_;
};

int main()
{
    {
        // scope_guard的作用之一，是让各种C风格指针接口作为局部变量时也能得到RAII支持
        // 这也是本题的基础要求
        FILE *fp = nullptr;
        try
        {
            fp = fopen("test.txt", "a");
            auto guard = scope_guard([&]
                                     {
                fclose(fp);
                fp = nullptr; });

            throw std::runtime_error{"Test"};
        }
        catch (std::exception &e)
        {
            puts(e.what());
        }
        assert(fp == nullptr);
    }
    puts("----------");
    {
        // 附加要求1，支持函数对象调用
        struct Test
        {
            void operator()(X *x)
            {
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
            struct Test
            {
                void f(X *&px)
                {
                    delete px;
                    px = nullptr;
                }
            } t;
            auto guard = scope_guard{&Test::f, &t, std::ref(x)};
        }
        assert(x == nullptr);
    }
}