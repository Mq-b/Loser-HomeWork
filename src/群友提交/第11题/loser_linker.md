```c++
#include <vector>

struct Pos {
    int x;
    int y;
};

int main(){
    std::vector<Pos> vec;

    vec.emplace_back(1, 5);
}
```

解释如下：  

```c++
template<class... Args>
constexpr reference emplace_back(Args&&... args)
```

emplace_back 通过 std::allocator_traits::construct 构造，它典型地用布置 new 于容器所提供的位置原位构造元素。  
参数 args... 以 std::forward<Args>(args)... 转发到构造函数。
c++20前，聚合体类型只能使用{}进行聚合初始化，但 c++20以后，也允许使用()进行聚合初始化，所以 c++20前，emplace_back 找不到构造函数，c++20以后可以找到聚合初始化，所以可以通过编译，  
也就是说 c++20以后，`Pos p(1,2);` 等同于之前的`Pos p{ 1,2 };`
