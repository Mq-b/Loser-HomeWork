 #include<iostream>
 //打什么表，直接在构造函数里面干它丫的
 template<class T>
 void runtime_align_offset_advance(std::size_t& layer, std::size_t &value, const std::size_t offset)
 {
     const std::size_t pack_size = alignof(T);
     const std::size_t half_pack = pack_size / 2;

     if (value + offset >= pack_size){
         const std::size_t advance_base = offset / pack_size;
         const bool has_rest = static_cast<bool>(offset % pack_size);
         const std::size_t advance_fix = has_rest ? advance_base + 1 : advance_base;

         layer += (((offset + value == pack_size) ? 0 : (value == 0 ? 0 : 1)) + advance_fix);
         value = 0;
     }
     else{
         value = ((value + offset > half_pack && offset >= half_pack) ? pack_size : (value + offset));
     }
 }

 template<class T> std::size_t runtime_align_offset_seek(const std::size_t pack_size, const std::size_t value, const std::size_t layer)
 {
     const std::size_t seek_offset = sizeof(T);
     std::size_t seek_layer{layer}, seek_value{};

     if (seek_offset + value >= pack_size){
         if (value != 0) {
             seek_layer += 1;
             seek_value = 0;
         }
     }else {
         seek_value = (seek_offset >= pack_size / 2) ? ((value == 0) ? 0 : (pack_size / 2)) : value;
     }
     return seek_layer * pack_size + seek_value;
 }

 //只要一转换就调用，构造函数会自动调用这个
 template<class F, class TT> struct init_and_call
 {
     F f;
     TT* x;
     inline static std::size_t value = 0;
     inline static std::size_t layer = 0;

     template<class T>
     operator T() {
         unsigned char* base_ptr = reinterpret_cast<unsigned char*>(x);
         const std::size_t offset = runtime_align_offset_seek<T>(alignof(TT), value, layer);

         f(*(reinterpret_cast<T*>(base_ptr + offset)));

         runtime_align_offset_advance<TT>(layer, value, sizeof(T));

         return T{};
     };
 };
//既然都不用结构化绑定了，size也不用求了吧，求了也只是重复一下上面的代码
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
     init_and_call<F, T>::value = 0;
     init_and_call<F, T>::layer = 0;
 }

 struct X
 {
    char cc{'k'};
    int a{10};
    char c{'l'};
    double b{2.33};
 };

 int main()
 {
    X x{};
    for_each_member([](auto a){ std::cout<<a<<' ';}, x);
 }
