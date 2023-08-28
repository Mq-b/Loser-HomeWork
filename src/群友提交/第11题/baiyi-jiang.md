## 11题
~~~   
#include <vector>

struct Pos {
    int x;
    int y;
};

int main(){
    std::vector<Pos> vec;
    vec.emplace_back(1, 5);
}
~~~
> 以下代码为什么在 C++20 以下的版本中无法成功编译，而在 C++20 及以后却可以

> 作答：   
C++20允许用括号形式的列表来进行聚合初始化，这种()初始化允许窄化转换，使用起来就像定义了有效的构造函数一样。