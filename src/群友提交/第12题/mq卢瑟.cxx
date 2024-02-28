#include <cstdio>
#include <vector>

inline void dbg(const char* msg)
{
    std::puts(msg);
    std::fflush(stdout);
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

constexpr auto make_vector(auto&& ...args) { return std::vector{ {std::forward<decltype(args)>(args)...} }; }

void test()
{
    static_assert(requires {
        {
            make_vector(std::vector{ 1, 2, 3 })
        } -> std::same_as<std::vector<std::vector<int>>>;
        {
            make_vector(1, 2, 3)
        } -> std::same_as<std::vector<int>>;
        make_vector(1, 2, 3).size() == 3;
    });
    X    x1;
    X    x2;
    auto vec = make_vector(x1, std::move(x2));
}

int main()
{
    test();
    dbg("test end");
}
