#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>


//ps宏展开在clang gcc下失败，以下是展开版
// https://gcc.godbolt.org/z/6Y37drM5K
// 为std::vector增加一个自定义的赋值函数
template <typename T>
    requires std::disjunction_v<std::is_integral<T>, std::is_floating_point<T>>
class vector : public std::vector<T> {
public:
    using std::vector<T>::vector;
    using std::vector<T>::size;
    using std::vector<T>::operator[];
    template <typename E>
    vector<T>& operator=(const E& e)
    {
        const auto count = std::min(size(), e.size());
        this->resize(count);
        for (std::size_t idx{ 0 }; idx < count; ++idx) {
            this->operator[](idx) = e[idx];
        }
        return *this;
    }
};


// 实现表达式模板类及相关函数

//macro:宏写的给我整懵了
#define make_vector_expr_declare(suffix, op) template<class E1, class E2> struct vector_expr_##suffix##;

#define make_vector_expr_cast(suffix, op) template<class EE2> auto operator##op##(EE2 const& e)\
{\
    return vector_expr_##suffix##<vector_expr<E1,E2>, EE2>{ *this, e };\
}\

#define make_vector_expr(suffix, op)\
template<class E1, class E2> struct vector_expr_##suffix## : public vector_expr<E1, E2>\
{\
public:\
    vector_expr_##suffix##(const E1& A, const E2& B) :vector_expr<E1, E2>(A, B) {}\
    double operator[](std::size_t i) const {\
        return vector_expr<E1, E2>::e1[i]##op##vector_expr<E1, E2>::e2[i];\
    }\
\
    template<class T> auto operator##op##(vector<T> const& v) const\
    {\
        return  vector_expr_##suffix##<vector_expr_##suffix##<E1, E2>, vector<T>>{*this, v};\
    }\
    template<class EE1, class EE2> auto operator##op##(vector_expr<EE1, EE2> const& B)\
    {\
        return vector_expr_##suffix##<vector_expr_##suffix##<E1, E2>, vector_expr<EE1, EE2>>{*this, B};\
    }\
};\

#define make_vector_expr_operator(suffix, op)\
template<class T> vector_expr_##suffix##<vector<T>, vector<T>> operator##op##(vector<T> const& A, vector<T> const& B)\
{\
    return { A, B };\
}\
template<class E1, class E2, class T> vector_expr_##suffix##<vector<T>,vector_expr<E1, E2>> operator##op##(vector<T> const& v, vector_expr<E1, E2> const& e)\
{\
    return { v, e };\
}\

make_vector_expr_declare(plus, +)
make_vector_expr_declare(sub, -)
make_vector_expr_declare(div, \)
make_vector_expr_declare(mul, *)



template<class E1, class E2>
struct vector_expr {

    std::remove_cvref_t<E1> const& e1;
    std::remove_cvref_t<E2> const& e2;

    vector_expr(const E1& e1_, const E2& e2_) :e1(e1_), e2(e2_){}
    std::size_t size() const { return 6; }
    virtual double operator[](std::size_t)const = 0;
    make_vector_expr_cast(plus, +)
    make_vector_expr_cast(sub, -)
    make_vector_expr_cast(div, /)
    make_vector_expr_cast(mul, *)

};


make_vector_expr(plus, +)

make_vector_expr(sub, -)

make_vector_expr(mul, *)

make_vector_expr(div, / )



make_vector_expr_operator(plus, +)

make_vector_expr_operator(sub, -)

make_vector_expr_operator(mul, *)

make_vector_expr_operator(div, /)




int main()
{
    auto print = [](const auto& v) {
        std::ranges::copy(v, std::ostream_iterator<std::ranges::range_value_t<decltype(v)>> { std::cout, ", " });
        std::cout << std::endl;
        };
    const vector<double> a{ 1.2764, 1.3536, 1.2806, 1.9124, 1.8871, 1.7455 };
    const vector<double> b{ 2.1258, 2.9679, 2.7635, 2.3796, 2.4820, 2.4195 };
    const vector<double> c{ 3.9064, 3.7327, 3.4760, 3.5705, 3.8394, 3.8993 };
    const vector<double> d{ 4.7337, 4.5371, 4.5517, 4.2110, 4.6760, 4.3139 };
    const vector<double> e{ 5.2126, 5.1452, 5.8678, 5.1879, 5.8816, 5.6282 };

    {
        vector<double> result(6);
        for (std::size_t idx = 0; idx < 6; idx++) {
            result[idx] = a[idx] - b[idx] * c[idx] / d[idx] + e[idx];
        }
        print(result);
    }
    {
        vector<double> result(6);
        result = a - b * c / d + e; // 使用表达式模板计算
        print(result);
    }
    return 0;
}
