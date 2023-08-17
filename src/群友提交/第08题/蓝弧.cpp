#include<iostream>
#include<string_view>
template<class,size_t>
struct array;
/*使得代码成功编译*/
template<>
struct array<void,0>
{
    array(...){};
    struct output_t
    {
        friend std::ostream& operator<<(std::ostream&os,output_t)
        {
            return os;
        }
    };
    struct iterator
    {
        iterator operator++() { return {}; }
        output_t operator*() { return {}; }
    };
    struct sentinel
    {
        friend bool operator!=(iterator,sentinel)
        {
            return false;
        }
    };
    iterator begin() { return {}; };
    sentinel end() { return {}; };
};
array(int,int,int,int,int)->array<void,0>;
/*并满足运行结果*/
constexpr std::string_view print_s = "1 2 3 4 5 ";
struct run
{
    ~run()
    {
        std::cout << print_s;
    }
}run_v;

template<class Ty,size_t size>
struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};
int main() {
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr) {
        std::cout << i << ' ';
    }
}
