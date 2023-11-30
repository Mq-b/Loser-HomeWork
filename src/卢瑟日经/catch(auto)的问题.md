# 起因

这问题倒不是来自卢瑟帝国，而是别的群聊，国王也决定稍微收录一下。

就是一位群友在群里说：

> catch 里面竟然可以用 auto 那和 ... 有啥区别

另一位群友回答：

> 函数参数也能用 auto 和三点
> 大概是类似的区别

**国王：**

>我说了两件事情：
>
>1. 不管是函数，还是 catch 中，... 和 auto 不一样，差很远。
>2. catch 中使用 auto，我闻所未闻，那就不会是标准有的，个人认为得是编译器扩展。

>msvc 可以通过编译，但是也没办法正常接取异常，msvc 的标准文档也没提这玩意

测试代码：

```cpp
int main()
{
    try
    {
        throw 1;
    }
    catch (auto)
    {
        
    }
}
```

gcc 和 clang 都[无法通过编译](https://godbolt.org/z/vxTW3c4sK)，但是实测我的 msvc 可以，但是它也没有办法接取这个异常，只是能通过编译而已，在运行时依旧会抛出异常。

![2](/Loser-HomeWork/image/卢瑟日经/catch(auto)02.png)

显然作用和 `...` 不一样。

咨询其他群友，意思很简单：

> 既然能通过编译，那试试打印类型。

![1](/image/卢瑟日经/catch(auto)01.png)

如果使用它，则无法通过编译，这种行为其实很像一个 bug。根本没初始化。没看出有什么意义和作用。

可以看看愚蠢的 GPT 的说法，因为这个问题就是 GPT 搞出来的。

![3](/Loser-HomeWork/image/卢瑟日经/catch(auto)03.jpg)

没有什么意义，按照它那样来

```cpp
#include <exception>
#include <iostream>

int main()
{
    try
    {
        throw std::runtime_error("乐");
    }
    catch (auto& e)
    {
        std::cout << typeid(e).name();
    }
}
```

一样是编译错误。

## 总结

暂时没啥结果，这种没有使用意义，应该不会是编译器扩展，可能就是个 bug。
