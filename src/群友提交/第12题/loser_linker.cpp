#include <cstdio>
#include <vector>
#include <type_traits>

template<typename T,typename... Ts>
auto make_vector(T&& t,Ts&&... ts)
{
    using vt = std::remove_cvref_t<std::common_type_t<T,Ts...>>;

    return std::vector<vt> {
        std::forward<vt>(ts)...
    };
}

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
    auto vec = make_vector(x1, std::move(x2));
}

int main()
{
    test();
    dbg("test end");

    return 0;
}