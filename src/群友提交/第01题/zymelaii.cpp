#include <type_traits>
#include <algorithm>

namespace detail {

template <typename T>
struct element_type {
    template <typename G>
    static constexpr auto check(int) -> typename G::value_type;

    template <typename G>
    static constexpr void check(...);

    using type = decltype(check<T>(0));
};

template <typename T>
struct value_type_substituter;

template <template <typename...> typename T, typename E, typename... Args>
struct value_type_substituter<T<E, Args...>> {
    //! FIXME: unsafe value_type substitution policy
    template <typename V>
    using type = T<V>;
};

} // namespace detail

template <typename T>
using element_type_t = typename detail::element_type<T>::type;

template <typename T, typename E>
using new_container_type_t =
    typename detail::value_type_substituter<T>::template type<E>;

namespace solution {

template <
    typename T,
    typename Fn,
    typename E = element_type_t<T>,
    typename   = std::enable_if_t<
        !std::is_same_v<E, void> && std::is_invocable_v<Fn, E>>>
auto operator|(const T& seq, Fn transform) {
    using value_type = std::invoke_result_t<Fn, E>;
    if constexpr (std::is_same_v<value_type, void>) {
        for (auto& e : seq) { transform(e); }
    } else {
        new_container_type_t<T, value_type> resp;
        std::transform(
            seq.begin(), seq.end(), std::back_inserter(resp), transform);
        return std::move(resp);
    }
}

} // namespace solution

#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    using namespace solution;

    std::vector<int> v{1, 2, 3};

    v | [](auto i) {
        return i * i;
    } | [](auto i) {
        std::cout << i << " ";
    };

    return 0;
}
