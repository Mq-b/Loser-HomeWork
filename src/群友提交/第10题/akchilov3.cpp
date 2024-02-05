#include<iostream>
#include<string>


 template<typename structT, typename T>
 std::size_t align_offset_seek(std::size_t& layer, std::size_t& offset)
 {
     constexpr std::size_t pack_size = alignof(structT);
     constexpr std::size_t offset_size = sizeof(T);
     if (offset % offset_size) {
         offset += 1;
         if (offset + offset_size > pack_size) {
             layer += 1;
             offset = 0;
         }
         return align_offset_seek<structT, T>(layer, offset);
     }

     return pack_size * layer + offset;
 }

 template<typename structT, typename T>
 void align_offset_advance(std::size_t& layer, std::size_t& offset)
 {
     constexpr std::size_t pack_size = alignof(structT);
     constexpr std::size_t advance_size = sizeof(T);
     if (offset + advance_size < pack_size) {
         offset += advance_size;
     }
     else {
         layer += advance_size / pack_size;
         layer += advance_size % pack_size ? 1 : 0;
         offset = 0;
     }
 }

 template<typename structT, typename T>
 T& read_from_align_offset(structT* base_ptr, const std::size_t offset)
 {
     return *(reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base_ptr) + offset));
 }
 template<class F, class TT> struct init_and_call
 {
     F f;
     TT* x;
     inline static std::size_t value = 0;
     inline static std::size_t layer = 0;

     template<class T>
     operator T() {
         const std::size_t off = align_offset_seek<TT, T>(layer, value);
         f(read_from_align_offset<TT, T>(x, off));
         align_offset_advance<TT, T>(layer, value);
         return T{};
     };
 };

 template<class F, class T, class first, class ...rest> constexpr void for_each_member_impl(F f, T* x)
 {
     if constexpr (!requires{ T{ first{}, rest{}... }; }) {
         T{ rest{f, x}... };
     }
     else {
         for_each_member_impl<F, T, init_and_call<F, T>, first, rest...>(f, x);
     }
 }

 template<class F, class T> constexpr void for_each_member(F f, T& x)
 {
     for_each_member_impl<F, T, init_and_call<F, T>>(f, &x);
 }

int main()
{
    struct X
    {
        char aaa{ 'A' };
        int aaaak{ 100 };
        std::string str{ "hello X!" };
        char cc{ 'k' };
        int a{ 10 };
        char c{ 'l' };
        short r{ 123 };
        char e{ 'u' };
        short rr{ 889 };
        int ac{ 19 };
        double b{ 2.33 };
        std::string str2{ "hello X again!" };
        double bbg{ 4.55 };
    };
    X x{};

    for_each_member([](auto&& i) {
        std::cout << i << ' ';
        }, x);
}
