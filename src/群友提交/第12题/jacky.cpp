#include <cstdio>
#include <vector>

inline void dbg(const char* msg)
{
    std::puts(msg);
    std::fflush(stdout);
}

template <class T, class... Arg>
    requires(std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<Arg>> && ...)
auto make_vector(T&& first, Arg&&... args)
{
    return std::vector<std::remove_cvref_t<T>>{
        {std::forward<T>(first), std::forward<Arg>(args)...}
    };
}

struct X {
    X() noexcept
    {
        dbg("X()");
    };

    ~X() noexcept
    {
        dbg("~X()");
    };

    X(const X&)
    {
        dbg("X(const X&)");
    }

    X(X&&) noexcept
    {
        dbg("X(X&&)");
    }
};

void test()
{
    static_assert(requires {
        {
            make_vector(std::vector{1, 2, 3})
        } -> std::same_as<std::vector<std::vector<int>>>;
        {
            make_vector(1, 2, 3)
        } -> std::same_as<std::vector<int>>;
        make_vector(1, 2, 3).size() == 3;
    });
    X    x1;
    X    x2;
    auto vec  = make_vector(x1, std::move(x2));
    auto vec2 = make_vector(std::vector{1, 2, 3});
}

int main()
{
    test();
    dbg("test end");
}
