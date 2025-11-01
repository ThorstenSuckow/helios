module;

#include <concepts>

export module helios.math.concepts:Numeric;

export namespace helios::math {

    /**
     * @brief Concept to constrain types to either integral or floating point.
     *
     * This concept should be used whenever template parameter constrain a
     * specific type to be a numeric type.
     */
    template<typename T>
    concept Numeric = std::integral<T> || std::floating_point<T>;
}
