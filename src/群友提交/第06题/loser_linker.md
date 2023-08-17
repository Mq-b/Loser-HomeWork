6能转换为std::atomic<int>，  
是因为调用了`constexpr atomic(T) noexcept;`用户自定义转换构造函数，  
相当于`std::atomic<int> n = std::atomic<int>(6);`  
在c++17之前的版本,通过调用复制或移动构造函数来完成n的初始化，  
标准规定std::atomic既不可复制亦不可移动，所以编译失败。  
c++17标准以后，复制清除变成强制要求，与此同时，用右值初始化对象不会再调用移动构造，而是直接原位构造，所以通过编译。