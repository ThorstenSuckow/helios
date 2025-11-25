module;

#include <type_traits>

export module helios.math.traits.FloatingPointType;



export namespace helios::math {

    /**
     * @brief Type trait that promotes integral types to double,
     * while preserving floating-point types.
     *
     * @tparam T the input type
     */
    template <typename T>
    using FloatingPointType = std::conditional_t<
        std::is_floating_point_v<T>,
        T,
        double
    >;


}