#include <algorithm>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>

// 为std::vector增加一个自定义的赋值函数
template <typename T>
    requires std::disjunction_v<std::is_integral<T>, std::is_floating_point<T>>
class vector : public std::vector<T> {
public:
    using std::vector<T>::vector;
    using std::vector<T>::size;
    using std::vector<T>::operator[];
    template <typename E>
    vector<T> &operator=(const E &e) {
        const auto count = std::min(size(), e.size());
        this->resize(count);
        for (std::size_t idx{ 0 }; idx < count; ++idx) {
            this->operator[](idx) = e[idx];
        }
        return *this;
    }
};

// 实现表达式模板类及相关函数
template<typename E1, typename E2>
struct vector_expr {
    const E1 &e1;
    const E2 &e2;

    vector_expr(const E1 &e1_, const E2 &e2_) : e1(e1_), e2(e2_) {}

    virtual double operator[](std::size_t idx) const = 0;

    std::size_t size() const { return this->e1.size(); }
};

template<typename E1, typename E2>
struct vector_add : vector_expr<E1, E2> {
    using vector_expr<E1, E2>::vector_expr;

    double operator[](std::size_t idx) const override { return this->e1[idx] + this->e2[idx]; }
};

template<typename E1, typename E2>
struct vector_sub : vector_expr<E1, E2> {
    using vector_expr<E1, E2>::vector_expr;

    double operator[](std::size_t idx) const override { return this->e1[idx] - this->e2[idx]; }
};

template<typename E1, typename E2>
struct vector_mul : vector_expr<E1, E2> {
    using vector_expr<E1, E2>::vector_expr;

    double operator[](std::size_t idx) const override { return this->e1[idx] * this->e2[idx]; }
};

template<typename E1, typename E2>
struct vector_div : vector_expr<E1, E2> {
    using vector_expr<E1, E2>::vector_expr;

    double operator[](std::size_t idx) const override { return this->e1[idx] / this->e2[idx]; }
};

template<typename E1, typename E2>
vector_add<E1, E2> operator+(E1 e1, E2 e2) {
    return { e1, e2 };
}

template<typename E1, typename E2>
vector_sub<E1, E2> operator-(E1 e1, E2 e2) {
    return { e1, e2 };
}

template<typename E1, typename E2>
vector_mul<E1, E2> operator*(E1 e1, E2 e2) {
    return { e1, e2 };
}

template<typename E1, typename E2>
vector_div<E1, E2> operator/(E1 e1, E2 e2) {
    return { e1, e2 };
}


int main() {
    auto print = [](const auto &v) {
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
