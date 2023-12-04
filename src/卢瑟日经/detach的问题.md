# detach的问题

## 起因

是在群里有群友问这个问题，[`std::thread::detach`](https://zh.cppreference.com/w/cpp/thread/thread/detach) 有什么作用？

## 解释

非常经典的日经问题，我无非就是回答：

> 从 thread 对象分离执行线程，允许执行独立地持续。一旦该线程退出，则释放任何分配的资源。

反正就是不保证实现，也可以看看 POSIX 的说法：

>*[pthread_detach()](https://pubs.opengroup.org/onlinepubs/009696899/functions/pthread_detach.html) 函数应该向实现表明，当线程终止时，线程的存储空间可以被回收。如果线程没有终止，pthread_detach() 将不会导致它终止。多个 pthread_detach() 调用对同一目标线程的影响是未指定的。*

>The pthread_detach() function shall indicate to the implementation that storage for the thread thread can be reclaimed when that thread terminates. If thread has not terminated, pthread_detach() shall not cause it to terminate. The effect of multiple pthread_detach() calls on the same target thread is unspecified.

当然了，这种问题显然不会这么单纯的聊，说说实现和举例子是必须的。

我给出了几段代码用于提问：

```cpp
int main() {
    std::string str{"C++11"};

    std::thread thr{[&str] { std::cout << str << '\n'; }};
    thr.detach();
}
```

这个引用捕获想都不用想可能有问题，str 的生存期有可能已经结束，下一个。

```cpp
int main() {
    std::string str{"C++11"};

    std::thread thr{[str] { std::cout << str << '\n'; }};
    thr.detach();
}
```

复制捕获就没问题了吗？

我的解释是参照了《C++CoreGuidelines解析》：

你需要关注的是 std::cout，它是一个全局对象，它的生存期如何呢？它什么时候析构？**全局对象的生存期应当和进程绑定**。

已经 `detach` 的线程，已经和主线程，进程，无关，所以，有可能主线程结束，进程结束，`std::cout` 对象已经析构了，此时运行已经 `detach` 的线程，有可能产生问题，因为它使用的 `std::cout` 已经析构了。

你认可这种说法吗？主要来自：

>*std::cout的生命周期绑定到进程的生命周期。这意味着线程可能在std::cout在屏幕上打印c++ 11之前就消失了。*
> std::cout’s lifetime is bound to the lifetime of the process. This means that the thread thr may be gone before std::cout prints C++11 onscreen.

其实我一开始是认可的，但是后面我想到了别的问题：

用户使用 `detach` 应该必然要求且保证，`detach` 的线程在主线程、进程执行完毕之前，就会结束。如果没有，那也不单单是 `std::cout` 会出现问题了。

以一段代码为例：

```cpp
#include <iostream>
#include<thread>
using namespace std::chrono_literals;

void f(){
    std::this_thread::sleep_for(10s); //确保主线程进程已经执行完毕
    system("mkdir luse2");
}

int main(){
    system("mkdir luse");
    puts("main");
    std::thread t{f};
    t.detach();
}
```

以上这段代码，在我的 `ubuntu22.04` 和 `windows11` 上（windows 得加 `powershell.exe`），都只能创建出 luse 文件夹，**而没有 luse2**。

那么如果确保执行已经 `detach` 的线程的时候，主线程和进程没结束呢？

```cpp
#include <iostream>
#include<thread>
using namespace std::chrono_literals;

void f(){
    system("mkdir luse2");
}

int main(){
    system("mkdir luse");
    puts("main");
    std::thread t{f};
    t.detach();
    std::this_thread::sleep_for(10s); //确保被 detach 的线程执行的时候还在
}
```

以上这段代码在 `ubuntu22.04` 和 `windows11` 上，都能成功创建出两个文件夹。
可见，问题就在于：
进程是否执行完毕**。

## 总结

这样倒是符合大部分人的使用要求，合理，但这是**无规定**。

文档对 `detach` 的实现要求很少，我们上面也列举了一点。

至于 《C++CoreGuidelines解析》 的解释（英文中文都一样），有一定道理，但并不算非常认可。毕竟当进程都结束的时候，被 `detach` 的线程可不单单是 `std::cout` 用不了，基本是啥也干不了了。

> 或许我们应该直接说：要确保 detach 的线程在主线程之前执行完毕？

另外聊一下 `detach` 使用这个事情，事实上我个人觉得，大多数时候就不该使用它，非常的不合适。

在正确维护的 C++ 中，根本不应该使用代码 `std::thread::detach` 。程序员必须确保所有创建的线程正常退出，释放所有获取的资源并执行其他必要的清理操作。这意味着通过调用 detach 放弃线程的所有权不是一种选择，因此 join 应该在所有场景中使用。

我知道还有各种理由和需求，老式接口和框架的设计，我知道，不用提，唉，事情总是不那么绝对，知道就好。
