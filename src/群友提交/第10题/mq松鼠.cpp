#include <iostream>

struct Init{
    template<class T>
    constexpr operator T(){return T{};}
};
template<int N>
struct tag:tag<N-1>{};

template<>
struct tag<0>{};


template<class T>
constexpr auto Size(tag<4>)-> decltype(T{Init(),Init(),Init(),Init()},0ull){
return 4ull;
}

template<class T>
constexpr auto Size(tag<3>)-> decltype(T{Init(),Init(),Init()},0ull){
return 3ull;
}

template<class T>
constexpr auto Size(tag<2>)-> decltype(T{Init(),Init()},0ull){
return 2ull;
}

template<class T>
constexpr auto Size(tag<1>)-> decltype(T{Init()},0ull){
return 1ull;
}

template<class T>
constexpr auto Size(tag<0>)-> decltype(T{},0ull){
return 0ull;
}

template<class T>
constexpr size_t size(){
    return Size<T>(tag<4>{});
}


template<class Ty,class Func>
void for_each_member(Ty& obj,Func& f){
    if constexpr (size<Ty>()==4){
        auto& [m1,m2,m3,m4]=obj;
        f(m1);f(m2);f(m3);f(m4);
    }
    else if constexpr (size<Ty>()==3){
        auto& [m1,m2,m3]=obj;
        f(m1);f(m2);f(m3);
    }
    else if constexpr (size<Ty>()==2){
        auto& [m1,m2]=obj;
        f(m1);f(m2);
    }
    else if constexpr (size<Ty>()==1){
        auto& [m1]=obj;
        f(m1);
    }
}
int main() {
    struct X { std::string s{ "loser king mq-b" }; }x;
    struct Y { double a{1}, b{1}, c{4}, d{5}; }y;
    std::cout << size<X>() << '\n';
    std::cout << size<Y>() << '\n';
    constexpr int s=size<X>();
    auto print = [](const auto& member) {
        std::cout << member << ' ';
    };
    for_each_member(x, print);
    for_each_member(y, print);
}