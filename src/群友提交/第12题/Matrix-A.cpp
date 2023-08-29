#include <cstdio>
#include <vector>

inline void dbg(const char* msg) {
    std::puts(msg);
    std::fflush(stdout);
}

struct X {
    X() noexcept {
        dbg("X()");
    };

    ~X() noexcept {
        dbg("~X()");
    };

    X(const X&) {
        dbg("X(const X&)");
    }

    X(X&&) noexcept {
        dbg("X(X&&)");
    }
};

// 直接宏替换，有没有可能标准库里没有make_vector就是因为可以用初始化器列表
//#define make_vector(...) std::vector{{__VA_ARGS__}}

// 用 emplace_back 函数调用换取拷贝时间
auto make_vector(auto&&... args) {
    std::vector<std::common_type_t<decltype(args)...>> temp;
    (temp.emplace_back(std::forward<decltype(args)>(args)),...);
    return temp;
}

void test() {
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

int main() {
    test();
    dbg("test end");
}
