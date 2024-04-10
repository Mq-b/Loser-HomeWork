# godbolt 使用文档

[**godbolt**](https://godbolt.org/) 是一个免费开源的 web 代码编译测试网站，可以帮助我们方便快速的进行代码的编译，可以选择众多的环境，更新快速，编译器版本众多。

godbolt 支持几乎市面上所有的编程语言以及主流编译器，不过**本文档使用 C++ 进行使用讲解**。

> **其实这个网站的名字叫做 Compiler Explorer，godbolt 是作者的名字，不过不管叫什么也不影响。**

## 打开 godbolt

每个人打开 godbolt 的布局样子可能完全不一样。

1. 如果你是第一次听说并且第一次打开这个网站，那么你的布局应该是固定的。
2. 如果你使用过，自己用过这个网站，自己设置了字体，或者其他布局，那么下次打开网站都是这样的。
3. 如果你打开过其他人分享给你的 godbolt 链接，**那么你的默认布局就会变成别人分享给你的样子**，自己重新打开 [**godbolt**](https://godbolt.org/) 刷新也是一样的。

所以我们先统一一下布局，以下是我分享的链接 ，直接打开即可：

```txt
https://godbolt.org/z/41eET6e5v
```

这样我们的布局就统一了：

![godbolt1](../../image/卢瑟日经/godbolt1.png)

目前的布局只有两个东西:代码文件、编译器。

我们看到它成功编译并且输出了运行结果。

## 初级使用

我们直接将其全部删除，再自己重新创建，点击上方控件的❌：

![godbot删除控件](../../image/卢瑟日经/godbolt删除控件.gif)

我们点击左上角的 **Add -> Source Editor** 得到了一个可以编辑的文本界面，可以放上自己的代码。

然后点击新建出来的这个代码编辑页的 **Add new -> Execution Only** 即可创建出一个执行单元，它与我们这个当前的文件关联，默认编译这个文件中的代码。

我们也可以点击 `A` ，左上角那个，设置代码的字体大小，然后写好代码，一个最基础的编译测试，就做好了：

![godbolt删除控件](../../image/卢瑟日经/godbolt创建控件.gif)

我们也可以选择别的编译器，或者增加一些别的编译选项：

![godbolt7](../../image/卢瑟日经/godbolt7.png)

那么如何分享给其他人看呢？点击右上角的 **Share -> Short Link**：

![godbolt5](../../image/卢瑟日经/godbolt6.png)

你就可以随意发送给其他人了

```txt
https://godbolt.org/z/scYjWvfjT
```

链接几乎是永久的，不用担心失效。

## 调整布局与主题设置

我们可以随意控制自己创建出的界面的字体大小和占据的位置：

![使用godbolt_gif](../../image/卢瑟日经/godbolt使用.gif)

当然，我们也可以设置自己喜欢的主题：

最上方，**More -> Settings -> Site theme**

![godbolt主题设置](../../image/卢瑟日经/godbolt主题设置.gif)

## 标准输入与命令行参数传递

```cpp
#include <iostream>

int main(){
    int n = 0;
    std::cin>> n;
    std::cout<< n <<'\n';
}
```

这样一段代码，如何让它在 godbolt 上运行并设置标准输入呢？很简单：

![godbolt标准输入](../../image/卢瑟日经/godbolt标准输入.gif)

分享链接：

```txt
https://godbolt.org/z/66Knd31oq
```

---

```cpp
#include <iostream>

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::cout << argv[i] << ' ';
    }
}
```

这样一段代码，我们如何通过 godbolt 传递给程序参数呢？很简单：

![godbolt输入命令参数](../../image/卢瑟日经/godbolt输入命令参数.gif)

分享链接：

```txt
https://godbolt.org/z/P8jxh4M47
```

## 多文件以及多编译器测试展示

一个 godbolt 网页中可以有多个代码文件，乃至多个编译器测试运行结果。

一个代码文件对应多个编译器测试运行结果：

![godbolt一个代码关联多个编译器测试](../../image/卢瑟日经/godbolt一个代码文件关联多个编译器测试.gif)

分享链接：

```txt
https://godbolt.org/z/sjeqdxKc7
```

如果要同时展示多段代码或者比较，那就按照之前的教程点击最左上角的 **Add -> Source Editor** 得到了一个可以编辑的文本界面，然后像上面一样 **Add new -> Execution Only** 即可创建出一个执行单元，设置编译器执行。如：

```txt
https://godbolt.org/z/hG17bvhrM
```

## 引入开源三方库

godbolt 几乎支持市面上所有常见的开源三方库，引入的方式也很简单。

```cpp
#include <iostream>
#include <fmt/core.h>

int main(){
    fmt::print("{}","😅");
}
```

这段代码使用到了 [**fmt**](https://github.com/fmtlib/fmt) 库，godbolt 想要编译运行它还需稍微设置一下：

![godbolt引入三方库](../../image/卢瑟日经/godbolt引入三方库.gif)

分享链接：

```txt
https://godbolt.org/z/nb6eTPeTK
```

## CMake 项目模板

godbolt 支持常见语言的项目模板，只需要点击网页最上方的 **Template** 选项即可。C++ 最常见的则是 CMake：

![godbolt打开CMake项目模板](../../image/卢瑟日经/godbolt打开CMake项目模板.gif)

然后按照正常的 CMake 用法，摸索一下即可。

引入三方库，设置主题，字体大小等，并无什么区别。

## Complier 控件的众多作用与设置

这个控件的用途就比较复杂了，它也可以像我们最初创建的 Execution Only 一样执行代码，不过功能还远不止如此。

### 创建 Complier

![godbolt创建Complier](../../image/卢瑟日经/godbolt创建Complier.gif)

创建还是很简单的，需要特别注意的是，默认没有标准输出，需要打开 Output 的 **Execute the code** 选项。

Complier **不支持传递标准输入以及命令行参数**，不过引入开源三方库还是和前面的没区别。如：

```txt
https://godbolt.org/z/K55Mh9j96
```

并且如你所见，创建 Complier 之后会显示我们代码生成的 intel 风格汇编。如果你喜欢 at&t 风格，可以点击 Output ，关闭默认的 **intel asm syntax** 选项。

### 查看名字重整名后的符号

```cpp
#include <iostream>

int a = 0;

namespace ss{
    int a = 0;
}

int main(){
    std::cout<<a<<'\n';
}
```

这样一段代码，我们如何查看 `a` 和 `ss::a` 的实际符号呢？

只需要点击 Output，关闭默认的 Demangle identifiers 选项即可：

![godbolt查看名字重整名的符号](../../image/卢瑟日经/godbolt查看名字重整名的符号.gif)

[Itanium ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi.html) 上变量 ss::a 的重整名为 _ZN2ss1aE，全局变量 a 则无变化。分享链接：

```txt
https://godbolt.org/z/cGE17r378
```

如果要测试多个编译器，那无非就是创建多个 Complier 即可。

如果你对重整名符号有疑问，请移步 [loser homework](https://github.com/Mq-b/Loser-HomeWork#14-%E4%BB%A5%E7%89%B9%E6%AE%8A%E6%96%B9%E6%B3%95%E4%BF%AE%E6%94%B9%E5%91%BD%E5%90%8D%E7%A9%BA%E9%97%B4%E4%B8%AD%E5%A3%B0%E6%98%8E%E7%9A%84%E5%AF%B9%E8%B1%A1)。

### 查看预处理后的代码

```cpp
#define max(a,b) (a)>(b)?(a):(b)
#define N 100

int main(){
    int arr[N]{};
    max(1,2);
}
```

想要查看这样一段代码的预处理后的结果，非常简单，添加 gcc 的编译选项 **`-E`** 即可：

![godbolt查看预处理后的代码](../../image/卢瑟日经/godbolt查看预处理后的代码.gif)

分享链接：

```txt
https://godbolt.org/z/xjGYh6zaE
```

## MSVC 的特殊性

如果你想测试 MSVC 编译器，没有问题，godbolt 支持，但是这是受限的：Add new -> Execution Only 创建的可执行单元是无法选择 MSVC 编译器的，必须是 **Add new -> Complier**。并且 Complier 你也没有办法看到 MSVC 运行输出结果，**只能看它是否可以通过编译**以及查看生成的汇编的信息之类的。

> 据传是曾经 MSVC 出了 bug ，然后选择 MSVC 编译器就不会再**运行**了程序了，只会编译而已。

分享链接：

```txt
https://godbolt.org/z/hv1ae87j7
```

## 总结

它的功能还远不止如此，不过大多数需求已经包含了。godbolt 的功能是非常强大的，还有更多的希望各位自行探索。

制作此文档最初目的是为了帮助更多新人使用 godbolt 进行交流，而不是动不动发一堆的代码。或者截图没代码等等问题。**希望各位开发者积极宣传使用 godbolt，如有疑问，把此文档发给他们**。或者您对此有什么补充，也欢迎提 pr 进行修改。

顺便我们再给几个常用的 C++ 工具网站：

- [cppinsights](https://cppinsights.io/) 用于查看编译器处理 C++ 语法之后的代码。
- [quick-bench](https://quick-bench.com/) 对比测试 C++ 代码效率。
- [build-bench](https://build-bench.com/) 对比测试 C++ 代码效率，用法比前面的简单。