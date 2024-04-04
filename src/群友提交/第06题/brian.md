为保证 atomic 对象的原子性，复制和移动都是不允许的。 
C++17之前 `std::atomic<int> n = 6;` 不合法是因为 atomic 只支持列表初始化， 写成 `std::atomic<int> n = {6};` 就可以通过编译了。
17+之后的 `std::atomic<int> n = 6;` 其实是原地构造，也不涉及任何复制移动，合法。
