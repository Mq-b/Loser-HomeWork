# 单例跨 `dll` 保证唯一定义吗？

单例的跨动态库（dll）是一个经久不衰的话题。尽管在中文互联网上有很多相关讨论，但许多回答存在一定的偏差或问题。在此，我们详细分析并澄清这一问题。

一个典型的 C++ 单例实现如下：

```cpp
inline my_class& get_my_class_instance(){
    static my_class instance; // C++11 起线程安全，且初始化严格发生一次
    return instance;
}
```

如果当前程序要编译为 dll，要将单例导出，不成问题，会确保只导出一个单例对象。最终出现问题的是别的程序引用了这个有单例定义的头文件，并使用这个 dll。C++ 标准无法管理这种情况，dll 不在管理的范围内，程序中此时可能出现多个单例。

这是一个简单且常见的实现形式，我们无需考虑其它的什么别的形式。这种单例实现的行为分为两种场景：静态库或同一项目中的翻译单元和动态库。

1. C++ 标准没有也不可能保证动态库的问题。
2. C++ 标准保证静态库，以及其它翻译单元。

## 具体来说

在内联函数中，所有函数定义中的函数局部静态对象在所有翻译单元间共享（它们都指代相同的在某一个翻译单元中定义的对象）。

例如在头文件中定义内联函数 `f`，其中定义静态局部对象 `n`，被多个 `.cpp` 文件 `include`，C++标准可以保证它们是同一个，在链接的时候进行处理，如何处理？链接时，编译器会消除重复的符号，确保唯一性。静态库同理。

而动态库截然不同，动态库是动态加载的，程序编译链接与它毫无关系（windows上有 `.lib` 的事，但本质上也和 `.dll`无关）。

如果想要多个 dll 共享一个单例对象，只能是把这个对象封装，编译为dll，提供接口，然后和其它东西一起使用。

## `so` 与 `dll`

再提一句，windows 和 Linux 的动态库，不单单是后缀名不同（`.so`、`.dll`）。

同时也代表了处理方式不同，windows 中隐式链接 dll 需要一个附带的 `.lib` 导入库文件，在给代码编译链接的时候会使用到 `.lib` 文件，因为它存放了符号。

具体来说，**编译器在编译一个个翻译单元的时候，经常会发现没有定义（这很正常），那么就会空着符号地址将其编译为目标文件（`.obj`、`.o`），等到链接的时候去其它翻译单元编译出的目标文件以及动态库静态库等等去寻找**。

`.lib` 文件的作用是告诉链接器某些符号存在于对应的 `.dll` 中，因此即使符号未定义，链接器也能正确链接生成可执行文件。只有在真正运行时，才会加载（`.dll`）动态库，提供符号的具体实现。

> Windows 中打包的 Release 程序你可能会注意到没有 `.lib` 文件，这是因为 `.lib` 仅在程序编译和链接时需要。生成了可执行文件后，符号信息已被记录，`.lib` 文件就没有什么实际用途了。

而 Linux 的 `.so` 动态库直接存储符号表信息。编译链接时，链接器会直接解析 `.so` 文件，查找符号定义；运行时再加载对应的 `.so` 文件提供实际实现。

## 参考

- <https://zh.cppreference.com/w/cpp/language/inline>

    >在内联函数中,
    >
    > + 所有函数定义中的函数局部静态对象在所有翻译单元间共享（它们都指代相同的在某一个翻译单元中定义的对象）。

- <https://zh.cppreference.com/w/cpp/language/storage_duration#.E9.9D.99.E6.80.81.E5.9D.97.E5.8F.98.E9.87.8F>

    > + 如果多个线程试图同时初始化同一静态局部变量，那么初始化严格发生一次（类似的行为也可对任意函数以 [`std::call_once`](https://zh.cppreference.com/w/cpp/thread/call_once) 来达成）。

- <https://learn.microsoft.com/zh-cn/windows/win32/dlls/about-dynamic-link-libraries>

    >在 DLL 中调用函数有两种方法：
    >
    > + 在*加载时动态链接*中，模块显式调用导出的 DLL 函数，就像它们是本地函数一样。 这要求将模块与包含函数的 DLL 的导入库链接。 导入库为系统提供加载 DLL 所需的信息，并在加载应用程序时查找导出的 DLL 函数。
    > + 在*运行时动态链接*中，模块使用 [`LoadLibrary`](https://learn.microsoft.com/zh-cn/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya) 或 [`LoadLibraryEx`](https://learn.microsoft.com/zh-cn/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibraryexa) 函数在**运行时加载 DLL**。 加载 DLL 后，模块调用 [`GetProcAddress`](https://learn.microsoft.com/zh-cn/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress) 函数以获取导出的 DLL 函数的地址。该模块使用 `GetProcAddress` 返回的函数指针调用导出的 DLL 函数。这样就不需要导入库了。

- <https://learn.microsoft.com/zh-cn/windows/win32/dlls/dynamic-link-library-creation>

    > **创建导入库**
    > 导入库 (`.lib`) 文件包含链接器解析对导出 DLL 函数的外部引用所需的信息，以便系统可以在运行时找到指定的 DLL 和导出的 DLL 函数。 生成 DLL 时，可以为 DLL 创建导入库。
    >
    > **使用导入库**
    > 例如，要调用 CreateWindow 函数，必须将代码链接到导入库 `User32.lib`。 这是因为 CreateWindow 驻留在名为 `User32.dll` 的系统 DLL 中，并且 `User32.lib` 是导入库，用于将代码中的调用解析为 `User32.dll` 中的导出函数。 链接器将创建一个表，其中包含每个函数调用的地址。 加载 DLL 时，将修复对 DLL 中的函数的调用。 当系统正在初始化进程时，由于该进程依赖于该 DLL 中的导出函数，因此它会加载 `User32.dll`，并会更新函数地址表中的条目。 对 CreateWindow 的所有调用都会调用从 `User32.dll` 中导出的函数。

- <https://docs.kernel.org/livepatch/module-elf-format.html>
