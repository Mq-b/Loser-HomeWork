#include <concepts>
#include <cstdio>
#include <functional>
#include <iostream>

template <class Func, class... Args>
    requires std::invocable<Func, Args...>
class scope_guard {
public:
    explicit scope_guard(Func&& func, Args&&... args) noexcept
        : m_func(std::forward<Func>(func))
        , m_args(std::forward<Args>(args)...)
    {
    }

    ~scope_guard() noexcept(std::is_nothrow_invocable_v<Func, Args...>)
    {
        std::apply(m_func, m_args);
    }

private:
    const Func                m_func;
    const std::tuple<Args...> m_args;
};

template <class Func, class... Args>
scope_guard(Func&&, Args&&...) -> scope_guard<Func&&, Args&&...>;

struct X {
    X()
    {
        puts("X()");
    }

    X(const X&)
    {
        puts("X(const X&)");
    }

    X(X&&) noexcept
    {
        puts("X(X&&)");
    }

    ~X()
    {
        puts("~X()");
    }
};

int main()
{
    {
        auto x     = new X{};
        auto guard = scope_guard([&] {
            delete x;
            x = nullptr;
        });
    }
    puts("----------");
    {
        struct Test {
            void operator()(X*& x)
            {
                delete x;
                x = nullptr;
            }
        };

        auto x = new X{};
        Test t;
        auto guard = scope_guard(t, x);
    }
    puts("----------");
    {
        struct Test {
            void f(X*& x)
            {
                delete x;
                x = nullptr;
            }
        };

        auto x = new X{};
        Test t;
        auto guard = scope_guard{&Test::f, &t, x}; // error
    }
}
