## Case 1

``` cpp
X f(){
    X x;
    return std::move(x);
}
```

这会阻止 NRVO，回退到 `return` 语句的自动移动。不建议使用。

## Case 2

``` cpp
X&& f(){
    X x;
    return std::move(x);
}
```

这返回了一个悬垂引用。`x` 具有在 `f()` 范围内的自动存储期，函数结束后就会离开生存期。

如果使用 UndefinedBehaviorSanitizer，程序会报错 `runtime error: reference binding to null pointer of type 'struct X'`。

## Case 3

``` cpp
struct Test {
    X x;
    X f() {
        return std::move(x);
    }
};
```

正确用法。`x` 不是“有移动资格”的，所以需要手动 move。
