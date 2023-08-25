"constexpr atomic(T desired) noexcept;" 是一个转换构造函数。
C++17之前 std::atomic < int>6是一个纯右值表达式 初始化的时候要进行 复制 / 移动构造，"std::atomic<int> n = 6" 可以写成 "std::atomic<int> n (std::atomic<int>(6))" 但是 atomic的复制构造被删除了。
而 C++ 17之后，纯右值表达式不在移动构造，而是原地构造。