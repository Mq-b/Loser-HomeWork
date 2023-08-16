#include<iostream>
template<class Ty,size_t size>
struct array {
    Ty* begin() { return arr; };
    Ty* end() { return arr + size; };
    Ty arr[size];
};
template<class Mq,class... Squirrel>
requires  (std::same_as<Mq,Squirrel> && ...)
array(Mq,Squirrel&&...) -> array<Mq,sizeof...(Squirrel)+1>;
int main() {
    ::array arr{1, 2, 3, 4, 5};
    for (const auto& i : arr) {
        std::cout << i << ' ';
    }
}