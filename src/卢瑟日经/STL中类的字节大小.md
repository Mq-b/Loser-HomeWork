# `STL` 中类的字节大小

“*STL 中 xx 类的大小是多少字节？*”这是一个非常**无趣**且**常见**的问题。你可以在非常多的地方见到，不管是技术论坛、QQ交流群、各种教学视频、面试八股。

然而，**大多数回答都是不准确的并且错误的想法众多**，或许我唯一觉得不错的回答是：“***实现定义***”。

当然，这回答肯定不是各位想要的，那我们就来稍微的详细聊一聊吧。

> 本节虽然是展开的聊广泛的情况，不过也可能会稍微聊一下具体标准库的具体类的源码实现，帮助各位更加直接的进行理解。

## STL 中的类它的字节大小和什么有关

这是多方面的，**但是和 C++ 标准没有什么直接关联，和编译器也没什么直接关系**。

基本在于五点：

- **标准库**
- **标准库版本**
- **Debug/Release**
- **32/64 位**
- **更加复杂的环境依赖**

> 我们不讲古老版本和现在的 STL 比较，那是浪费时间。光下面这些内容和可能性就够多了。

目前常见的只有三种 C++ 标准库，[`MSVC STL`](https://github.com/microsoft/STL)、 [`libstdc++`](https://github.com/gcc-mirror/gcc/blob/7a01cc711f33530436712a5bfd18f8457a68ea1f/libstdc%2B%2B-v3/include/std/) 、[`libc++`](https://github.com/llvm/llvm-project/blob/7ac7d418ac2b16fd44789dcf48e2b5d73de3e715/libcxx/include/)。基本也视作对应三个编译器，MSVC、GCC、clang，当然了，clang 也能用 libstdc++ 乃至 MSVC STL，不过这不重要。

> 我知道这里一定有人挑刺，想要纠正本人对于 STL 和标准库的理解，表达 STL 和标准库不是一个东西。然而我想告诉各位并没有严格定义，并且具体情况具体分析，在本节内容，这不重要。

## 举例 `std::vector`

```cpp
sizeof(std::vector<int>)
```

即使是这么常见的，各家标准库实现几乎并无太大不同的 `std::vector` ，其所占字节大小也有**至少四种**可能：**`12`**、**`24`**、**`16`**、**`32`**。

> 测试[链接](https://godbolt.org/z/7f34crzMz)。godbolt 没办法让 clang 使用 MSVC STL，所以使用 vs 安装了 `clang-cl` 16 进行本地测试。

产生这四种结果有两个原因：

1. 32与64位环境指针的大小不同
2. MSVC STL 与另外两个标准库不同，在 debug 的情况下多了一个迭代器。

**这个结果无关编译器**，你的 clang 也完全可以运行出这 4 种结果，因为你的 clang 也能用 MSVC STL。

如果使用 MSVC 或 clang 编译器完全可能是以上四个任意一个结果，所以本人非常厌恶不明确的问题，最终沦为愚蠢的不带脑子的八股进行背诵。如果要由你们自己补充题目再完全正确的回答这个问题，要求的知识水平又过高了。

---

## 举例 `std::string`

```cpp
sizeof(std::string)
```

`std::string` 的可能性就更多了，它的实现还有一个 `SSO` 缓冲。其所占字节大小**至少**也有**四种**可能：`24`、`28`、`32`、`40`。

这四个结果的原因和前面一样。

## 举例 `std::thread`

```cpp
sizeof(std::thread)
```

这个类我们就稍微详细聊聊吧，毕竟是较为简单的独立类。

---

**MSVC STL** 的 `std::thread` 实现是只保有一个私有数据成员 [`_Thr`](https://github.com/microsoft/STL/blob/8e2d724cc1072b4052b14d8c5f81a830b8f1d8cb/stl/inc/thread#L163)：

```cpp
private:
    _Thrd_t _Thr;
```

[`_Thrd_t`](https://github.com/microsoft/STL/blob/8e2d724cc1072b4052b14d8c5f81a830b8f1d8cb/stl/inc/__msvc_threads_core.hpp#L20-L24) 是一个结构体，它保有两个数据成员：

```cpp
using _Thrd_id_t = unsigned int;
struct _Thrd_t { // thread identifier for Win32
    void* _Hnd; // Win32 HANDLE
    _Thrd_id_t _Id;
};
```

结构很明确，这个结构体的 `_Hnd` 成员是指向线程的句柄，`_Id` 成员就是保有线程的 ID。

在64 位操作系统，因为内存对齐，指针 8 ，无符号 int 4，这个结构体 `_Thrd_t` 就是占据 16 个字节。也就是说 `sizeof(std::thread)` 的结果应该为 **16**。

---

`libstdc++` 的实现则不同，它为了支持更多平台，使用了一些宏进行兼容，但是它最终保有私有数据成员是：

```cpp
private:
    id            _M_id;
```

id 就是 libstdc++ 实现的 [`std::thread::id`](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/std_thread.h#L92-L120)，那么问题继续，这个 `std::thread::id` 类型又是保有了什么数据成员呢？

```cpp
native_handle_type  _M_thread;
```

它只保有了这样一个成员，`native_handle_type` 是定义的别名，它是：

```cpp
#ifdef _GLIBCXX_HAS_GTHREADS
    using native_handle_type = __gthread_t;
#else
    using native_handle_type = int;
#endif
```

那么，`__gthread_t` 又是什么？

```cpp
typedef __gthr_win32_HANDLE __gthread_t;
typedef void *__gthr_win32_HANDLE;
```

所以其实是 `void*`，那么问题就解决了。

`libstdc++` 实现的 `std::thread` 要吗相当于保有了一个 `void*`，要吗是 `int`，至于大小也不一定，32 位的话自然指针也是 4 字节。

**大多数情况下你 `sizeof(std::thread)` 会为 `8`**，也就是 `_GLIBCXX_HAS_GTHREADS` 宏被定义，即有 [`GThread`](https://docs.gtk.org/glib/struct.Thread.html)。

> 实测 windows11 环境，gcc 不管是 POSIX 还是 win32 线程模型，都不重要，64位下 `sizeof(std::thread)` 都会为 `8`。测试 wsl 中的 gcc 也是一样。

---

[`libc++`](https://github.com/llvm/llvm-project/blob/6f6336858e4588ebd113ebcc930f6384a4edca54/libcxx/include/__thread/thread.h#L148) 就不再介绍了，有兴趣各位可以自己慢慢看。

---

如果有兴趣，建议阅读 [`std::thread` 的构造-源码解析](https://github.com/Mq-b/ModernCpp-ConcurrentProgramming-Tutorial/blob/main/md/%E8%AF%A6%E7%BB%86%E5%88%86%E6%9E%90/01thread%E7%9A%84%E6%9E%84%E9%80%A0%E4%B8%8E%E6%BA%90%E7%A0%81%E8%A7%A3%E6%9E%90.md)。

## 总结

我们讲的情况并不是全部，比如根本没对比古老实现的的 STL 库的结果。但是，这不影响，因为本文要表达的意思到了。

- **提问者应当说清楚问题，让其具体且没有歧义**。

尤其讨厌 2024 年了，提问的语境还默认什么 gcc4.9 ，你是在和我开玩笑吗？或许手里拿的八股答案就是这玩意。

本文也不是为各位介绍分析 STL 中那些类的字节大小、实现原理。而且不用感到奇怪，主旨就是：

- **具体问题具体分析**。

并且如果你有兴趣自己分析这些问题，那么你显然需要阅读源码。然而**不会模板，阅读标准库源码，是无稽之谈**。这里推荐一下：[**现代C++模板教程**](https://github.com/Mq-b/Modern-Cpp-templates-tutorial)。
