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
    vector<T>& operator=(const E& e) {
        const auto count = std::min(size(), e.size());
        this->resize(count);
        for (std::size_t idx{ 0 }; idx < count; ++idx) {
            this->operator[](idx) = e[idx];
        }
        return *this;
    }
};

/*
* 如代码所示，表达式模板 和 std::ranges::zip_transform_view 实现了类似的内容。
* ranges 库中的实现更复杂，如包含迭代器的实现、更复杂的使用环境等;
* 自己实现 表达式模板 可以省去部分未使用的代码实现，提升一丝丝性能（绝大部分情况下没有必要，直接用 ranges 库就好）；
*/

// 使用宏控制使用 「表达式模板」还是「范围库」
#define use_expr true

template<typename F, typename L, typename R>
    requires std::is_invocable_v<F, typename L::value_type, typename R::value_type>
struct vector_expr {
    using value_type = std::invoke_result_t<F, typename L::value_type, typename R::value_type>;

    const F& func;
    const L& lhs;
    const R& rhs;

    std::size_t size() const {
        return std::min(lhs.size(), rhs.size());
    }
    inline auto operator[](const size_t idx) const {
        return func(lhs[idx], rhs[idx]);
    }
};

template<typename V>
concept is_vector = std::is_same_v<V, vector<typename V::value_type>>;

template<typename E>
concept is_expr = std::is_same_v<E, vector_expr<
    std::remove_cvref_t<decltype(E::func)>,
    std::remove_cvref_t<decltype(E::lhs)>,
    std::remove_cvref_t<decltype(E::rhs)>
    >>;

template<typename T>
concept is_ranges_expr_vector = is_vector<T> || is_expr<T> || std::ranges::range<T>;


template<is_ranges_expr_vector L, is_ranges_expr_vector R>
auto operator+(const L& lhs, const R& rhs) {
#if use_expr
    return vector_expr{ std::plus{}, lhs, rhs };
#else 
    return std::ranges::zip_transform_view(std::plus{}, lhs, rhs);
#endif
}

template<is_ranges_expr_vector L, is_ranges_expr_vector R>
auto operator-(const L& lhs, const R& rhs) {
#if use_expr
    return vector_expr{ std::minus{}, lhs, rhs };
#else 
    return std::ranges::zip_transform_view(std::minus{}, lhs, rhs);
#endif
}

template<is_ranges_expr_vector L, is_ranges_expr_vector R>
auto operator*(const L& lhs, const R& rhs) {
#if use_expr
    return vector_expr{ std::multiplies{}, lhs, rhs };
#else 
    return std::ranges::zip_transform_view(std::multiplies{}, lhs, rhs);
#endif
}

template<is_ranges_expr_vector L, is_ranges_expr_vector R>
auto operator/(const L& lhs, const R& rhs) {
#if use_expr
    return vector_expr{ std::divides{}, lhs, rhs };
#else 
    return std::ranges::zip_transform_view(std::divides{}, lhs, rhs);
#endif
}


int main() {
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
