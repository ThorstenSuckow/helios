/**
 * @file TypedTupleCat.ixx
 * @brief Compile-time concatenation utility for std::tuple type lists.
 */
module;

#include <tuple>

export module helios.core.TypedTupleCat;

export namespace helios::core {

    /**
     * @brief Concatenates multiple `std::tuple` type lists at compile time.
     *
     * @details Produces a single `std::tuple<...>` containing all types from
     * the input tuples in their original order.
     *
     * @tparam Tuples Input tuple type lists.
     */
    template<typename... Tuples>
    struct TypedTupleCat;

    /**
     * @brief Base specialization for a single tuple.
     *
     * @tparam Ts Element types of the tuple.
     */
    template <typename... Ts>
    struct TypedTupleCat<std::tuple<Ts...>> {
        using type = std::tuple<Ts...>;
    };

    /**
     * @brief Recursive specialization that merges the first two tuples and continues.
     *
     * @tparam Ts Element types of the first tuple.
     * @tparam Us Element types of the second tuple.
     * @tparam Rest Remaining tuples to concatenate.
     */
    template <typename... Ts, typename... Us, typename... Rest>
    struct TypedTupleCat<std::tuple<Ts...>, std::tuple<Us...>, Rest...> {
        using type = typename TypedTupleCat<std::tuple<Ts..., Us...>, Rest...>::type;
    };

};

