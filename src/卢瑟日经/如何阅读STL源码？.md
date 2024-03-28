# 如何阅读 `STL` 源码

很多人谈起 `STL` 源码，脑子里就只能一些上古的教材教程，以及古老的版本。

当然了，他们可能会狡辩：***“学习设计原理，和版本无关”*** 等等说法，是，也不是，没有必要。

新人尤其**不要**去寻找一些莫名其妙的上古版本的 `STL` 然后单独阅读 ，那样很愚蠢。

## 明确你的目的

我们要达到的是能够“阅读 STL 源码”，而不是说什么这里道听途说：***xxx 容器底层是 xxx，是怎么实现的 xxx。然后又看某些上古实现，迷迷糊糊，模板一堆的语法不懂，看一点猜一点。一旦你陷入到这种，算是完了，直接重开吧*** 。

你要看源码，就不可能不看模板，**你必须学习模板**，不需要会太多的元编程等等的玩意，但是基本的需要会。**而且源码推荐看越新的越好**。

并且只建议看三种：[**`MSVC STL`**](https://github.com/microsoft/STL)、[**`libstdc++`**](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)、[**`libc++`**](https://github.com/llvm/llvm-project/tree/7ac7d418ac2b16fd44789dcf48e2b5d73de3e715/libcxx)，也就是对应了三大编译器，MSVC、gcc、clang。不要舍本逐末，你应该看的就是最常用最常见的实现代码，而且要是最简单的方式，比如在开发环境中就能直接跳转进去。

这三个标准库中，`MSVC STL` 与 `libstdc++` 最为常见，clang 默认也是使用 `libstdc++`，**个人而言最推荐的是 `MSVC STL`** 的。

尤其如果你正在使用 MSVC 作为你的编译器，那就不用犹豫了。

## 阅读现代的 `STL` 源码

您都阅读源码了，相信也是有所追求，既然如此，自然越新越好，尤其 MSVC STL。

MSVC STL 很早之前就不支持 C++11 了，它的实现完全基于 **C++14**，出于某些原因 **C++17** 的一些库（如 [`invoke`](https://zh.cppreference.com/w/cpp/utility/functional/invoke)， _v 变量模板）被向后移植到了 **C++14** 模式，所以即使是 C++11 标准库设施，实现中可能也是使用到了 C++14、17 的东西。

所以您至少要学习或具备 `C++17` 的知识才能阅读它的源码。

## 观感

我本人对于 libstdc++ 那种规范是深恶痛绝，一行代码不能超过 80 个字符，一堆莫名其妙的格式，我们来对比一下就好了：（主题：OneDarkPro 字体：Cascadia Code SemiBold 字号：16）

![MSVC STL](/image/卢瑟日经/MSVC-STL.png)

字体：Menlo 字号：16

![libstdc++](/image/卢瑟日经/libstdc++.png)

嗯... 或许因为我的主题和美化做的太好了，不晓得各位能不能感觉。

## 总结

阅读御三家的 STL 源码，尤其推荐 MSVC STL，libstdc++ 次之。

需要学习模板，这是必须的，建议学习[《现代C++模板教程》](https://mq-b.github.io/Modern-Cpp-templates-tutorial/)

阅读**现代**的 STL 源码，这个是必须的。

阅读观感这个因人而异了，反正我个人而言是更喜欢 MSVC STL。
