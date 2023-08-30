`std::vector<T>::emplace_back()` **最终**会使用类似于如下的代码构造对象：

```cpp
::new (static_cast<void*>(p)) T(std::forward<Args>(args)...)
```

很明显，这里使用的是[直接初始化](https://zh.cppreference.com/w/cpp/language/direct_initialization)（[*direct initialization*](https://en.cppreference.com/w/cpp/language/direct_initialization)）。

在 C++20 种，加入了“括号形式的聚合体初始化”（cppref 说法，仅便于理解用，并不准确）具体为：

> 如果目标类型是（可能有 cv 限定）的聚合类，则按聚合初始化中所述进行初始化，但允许窄化转换，不允许指派初始化器，不延长引用所绑定到的临时量的生存期，不进行花括号消除，并值初始化任何无初始化器的元素。**（C++20 起）**
>
> ```cpp
> struct B
> {
>     int a;
>     int b;
> };
>  
>  
> B b1{1, 2};            // OK
> B b1(1, 2);            // 也 OK（C++20 起）
> ```

因此上面的那种方法可以用于构造聚合类。而 C++20 以前不能这么做，所以无法通过编译。
