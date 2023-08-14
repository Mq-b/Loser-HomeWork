#include <iostream>

template<class Ty, size_t size>
struct array
{
    Ty* begin() { return arr; }
    Ty* end() { return arr + size; }
    Ty arr[size];
};

/**
 * ģ����������Ƶ�ָ�� + �Ƶ���������(std::common_type_t)
 */
template<class... T>
array(T...) -> ::array<std::common_type_t<T...>, sizeof...(T)>;

int main()
{
    ::array arr{1, 2, 3, 4, 5};

    for (const auto& i : arr)
    {
        std::cout << i << ' ';
    }
}