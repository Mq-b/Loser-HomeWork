#include <cstdint>
#include <iostream>
template <class Ty, size_t size>
struct array
{
    Ty *begin() { return arr; };
    Ty *end() { return arr + size; };
    Ty arr[size];
};

template<class Ty,class...Args>
array(Ty,Args...)->array<Ty,1+sizeof...(Args)>;

int main()
{
    ::array arr{1, 2, 3, 4, 5};
    for (const auto &i : arr)
    {
        std::cout << i << ' ';
    }
}