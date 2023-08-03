给出代码：
```
int main(){
    std::vector v{1, 2, 3};
    std::function f {[](const int& i) {std::cout << i << ' '; } };
    auto f2 = [](int& i) {i *= i; };
    v | f2 | f;
}
```

要求在不更改给出的main函数代码的前提下增加代码，使其运行结果为：
1 4 9

提交方式：代码截图与运行结果
难度：一星
