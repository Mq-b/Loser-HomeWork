# 关于possibilities
由于我写的太抽象了，所以写个文档解释下
## 定义
这个工具主要使用该命名空间下的类型别名`possibility`
```cpp
template<class T, class Poss> 
using possibility = typename make_construct_list<
    possibilities::size<T>() - 1, 
    T, Poss, tl<>
>::type;
```
其中T为所要猜测的聚合体， Poss为单词possibility的缩写，即可能的类型列表。
返回(如果成功)：possibilities::tl<T的成员类型...>。
如果失败，则编译失败

## 用法展示
possibilities 的原理是给出一个聚合体的成员类型的可能，用std::is_constructible来猜测聚合体的成员类型， 其用法如下：
```cpp
struct X{ int a; double b;};
struct Y{int a; std::string s; char c;};

//定义所要猜测的聚合体类型T中的可能的成员类型， 不一定是精确的
//但是T中所有的成员类型一定要在下面的列表中出现至少一次
//T中不能包含聚合体成员
//possi不一定是possibilities::tl, 可以是任意的类型列表，但是不能出现非类型模板参数
using possi = possibilities::tl<int, double, char, std::string,
/*无关类型*/bool, long, float, unsigned char, /.../std::vector<std::string>>;

int main()
{
    //猜测X的成员类型
    using XL = possibilities::possibility<X,possi>;
    //T = possibilities::tl<int ,double>
    //猜测Y的成员类型
    using YL = possibilities::possibility<Y,possi>;
    //U = possibilities::tl<int ,std::string ,char>
}
```
## 实现
possibilities使用了TMP，其实是比较老式的写法了，主要是进行类型计算， 以下是所有类模板的解释：

### namespace possibilities_utilities
这个命名空间里面包含实现`possibilities的工具模板，目前暂时没发现stl有提供类似的元编程工具，只能手动写。

#### struct `init`
这个工具主要类计算类型的成员的数量，和loserhomework中第十题的绝大多数用法一样

#### template<typename T> `struct convert_forbid`
和init的用法一样，但禁止隐式转换，也就是说，当构造函数要求的类型是int， 那么struct convert_forbid::template operator T一定要实例化为int，
其它类型例如double什么的转换成int是禁止的，主要是用来确定T中的第N个成员是不是某种类型，例如(使用上述示例中的结构体X)：
```cpp
std::is_constructible_v<X, init, convert_forbid<double>>;//true
std::is_constructible_v<X, init, convert_forbid<int>>;//false
```
如不使用此模板， std::is_constructible会认为int， double是convertible的， 而都返回true
#### template<typename T, typename ...Inits> `struct count_size`
用模板递归求解类型T中成员的数量， 并且提供一个用这个数量构成的init类型列表：
- count_size<...>::value 返回成员的数量
- typename count_size<...>::CTL 得到T和`value`个`init`所组成的类型列表tl<T, init...>

#### template<...> `struct replace_at`
用模板递归替换类型列表中的的第N个类型为新的类型，由于不对外提供，所以不提供简化的别名调用，主要用convert_forbid<T> 来替换 `typename countsize<...>::CTL` 中的类型，
用来生成测试。
- typename replace_at<...>::type;

#### template<template<typename...> typename my_alias> `struct as_`
用来替换别名
- typename as_<my_alias>::type;
```cpp
using b = as_<A<int, double>, B>::type;//b = B<int, double>;
```

#### template<std::size_t N, typename TL> `struct select_element`
用as_把一个类型列表变成std::tuple<...>, 来骗std::tuple_element_t以获得类型列表中的第N个类型（其实我不知道用不用骗，但还是转换一下保险）
- typename select_element<N, TL>::type

### possibilities的主体实现

#### template<typename type> constexpr `std::size_t size()`
特化了`count_size`，最多支持聚合体中有15个成员的成员数量计算，想要更多可以加init的数量和模板参数中的size_t 参数

#### template<std::size_t N, typename T, typename mem_type> `struct is_constructible_at`
用来判断第T的第N个成员是不是`mem_type`， 原理是用`replace_at` 将`count_size`的`CTL`中的第N个init替换为`convert_forbid<mem_type>`，然后用`as_`把CTL变成类似
`struct is_constructible<T, init..., convert_forbid<mem_type>, init...>`, 然后求value, 当value为true是表明mem_type是T的第N个成员类型，反之则不是。

#### template<bool constructible_, std::size_t N, typename T, mem_type_list<...>> `struct constructible_at_try`
用给定的类型列表`mem_type_list<...>`, 用模板递归的方式调用`is_constructible_at`,逐个测试`mem_type_list<...>`中所有的类型，其返回的别名type为T的第N个成员的成员类型，
在这里，假如都匹配不上，就会编译失败，因为递归的中止条件为`is_constructible_at`的value为true， 假如一直不是true就会因为溢出`replace_at`而爆炸。

#### template<...> `struct make_construct_list`
又用模板递归调用`constructible_at_try`, 由于`constructible_at_try`会得到某个聚合类的第N个成员的类型，所以递归生成从0到第N个的成员类型，生成一个类型列表，
用possibilities::tl来保存并返回

## 番外`align_offset_ptr`
定义
```cpp
template<std::size_t pack_size, std::size_t layer_offset, std::size_t offset> 
struct align_offset_ptr{...};
```

其中：
- pack_size为内存对齐长度
- layer_offset 为当前所处的内存层
- offset为当前所处内存层的偏移值

模拟在某种对齐方式下各种数据的分布情况，它将结构体的内存看成是一层层不超过对齐限制的内存层， 在这些内存层中寻找某种类型的数据的首地址（一个偏移值），它是在编译期计算的，
并且使用了模板递归来避免大量函数递归，这让它的使用方式很复杂，但可以使用封装的
`auto & get_member<std::size_t N, typename CTL,  typename structT>(structT* base_ptr)` 
快捷使用，
CTL(construct type list)可以通过possibilities得到。

如不使用get_member, 可以使用 `align_offset_ptr::layer` * pack_size + `align_offset_ptr::value` 得到成员的偏移值

align_offset_ptr的寻址分为两个部分：
- typename align_offset_ptr::template seek<std::size_t seek_size> 寻找某种size的类型基于当前偏移值的首地址偏移
- typename align_offset_ptr::template advance<std::size_t advance_size> 越过某个类型的长度后的偏移

seek 和 advance 都递归返回特定的align_offset_ptr, 但是拥有不同的偏移值。

`align_offset_ptr` 可以使用`offset_iter`元迭代器，在给定的类型列表中行进,这个迭代器会自动处理seek 和 advance 的过程. 实际上`get_member`就是用了这个迭代器.
其定义如下：
```cpp
 template<std::size_t N, typename align_offset_pointer, typename TL> struct offset_iter {...};
```

示例（还是刚刚哪个例子中的Y，注意，std::string 在不同的编译器中有不同的size，这个结果仅在msvc中）：
```cpp
using Y_ptr2 = typename offset_pointer::offset_iter<2,
    offset_pointer::align_offset_ptr<alignof(Y), 0, 0>,
    YL>::iter_type;
using Y_char_ptr = typename Y_ptr2::template seek<sizeof(char)>;//Y_char_ptr = struct align_offset_ptr<8, 6, 0>
```
在上述的例子中，align_offset_ptr越过了Y 中的int, string, 并使用seek得到Y中char的偏移值为6 * 8 + 0 即 48.
可以使用函数 `read_from_align_offset` 应用这个偏移值在某个Y的实例中。

需要注意的是， align_offset_ptr的计算是无关类型的，除了内置的静态函数get需要类型信息，因此可以无限 advance 和 seek


## 总结
用暴力匹配来解析聚合体中的成员类型，没啥技术含量。综合possibilities提供的类型信息和align_offset_ptr的寻址能力，我写出了第十题的for_each_member，试图寻找不使用结构化绑定的解法（因为别人都这么写了）。但仍有诸多限制。
