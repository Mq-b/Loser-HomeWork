#include <concepts>
#include <cstdio>
#include <functional>
#include <iostream>

template<typename F,typename... Ts>
concept callable = requires(F&& f,Ts&&... ts){
    std::invoke(f,ts...);
};

template <class Func, class... Args>
    requires callable<Func,Args...>
class scope_guard {
    Func func_;
    std::tuple<Args...> args_;
public:
    template<typename F,typename... Ts>
    scope_guard(F&& func, Ts&&... args)
        : func_{ std::forward<Func>(func) }
        , args_{ std::forward<Ts>(args)... }
    {
    }

    ~scope_guard()
    {
        std::apply(func_, args_);
    }

    scope_guard(scope_guard const &) = delete;
    scope_guard& operator=(scope_guard const &) = delete;
};

template <class F, class... Ts>
scope_guard(F&&, Ts&&...) -> scope_guard<std::decay_t<F>, std::decay_t<Ts>...>;

struct X {
    X(){ puts("X()"); }

    X(const X&){ puts("X(const X&)"); }

    X(X&&) noexcept{ puts("X(X&&)"); }

    ~X(){ puts("~X()"); }
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
