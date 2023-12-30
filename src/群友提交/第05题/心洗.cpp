import std;

template <typename... T>
    requires std::invocable<T...>
class scope_guard {
public:
    constexpr explicit scope_guard(T&&... args)
        : package_task(std::bind(
            std::forward<decltype(args)>(args)...))
    {
    }

    ~scope_guard() noexcept
    {
        package_task();
    }

private:
    const std::function<void()> package_task;
};

template <class Func, class... Args>
scope_guard(Func&&, Args&&...) -> scope_guard<Func&&, Args&&...>;

struct X {
    X()
    {
        std::cout << "X()\n";
    }

    X(const X&)
    {
        std::cout << "X(const X&)\n";
    }

    X(X&&) noexcept
    {
        std::cout << "X(X&&)\n";
    }

    ~X()
    {
        std::cout << "~X()\n";
    }
};

int main()
{
    {
        auto x = new X{};
        auto guard = scope_guard([&] {
            delete x;
            x = nullptr;
            });
    }
    std::cout << "----------\n";
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
    std::cout << "----------\n";
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
        auto guard = scope_guard{ &Test::f, &t, x }; // error
    }
}