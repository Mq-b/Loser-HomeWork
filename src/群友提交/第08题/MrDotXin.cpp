/* 第八题
    群ID: 还没想好名字
*/
#include <iostream>
template<typename Ty, std::size_t size>
struct array {    
    Ty * begin() {return arr;}
    Ty * end() {return arr + size;}
    Ty  arr[size];
};

template<typename...Ty>
array(Ty...args) -> array<std::common_type_t<Ty...>, sizeof...(args)>;

int main() 
{
    ::array arr {1,2,3,4,5};
    for (const auto& v : arr) {
        std::cout << v << ' ';
    } 
    return 0;
}
