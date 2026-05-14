module;

#include <concepts>

export module helios.math.concepts:IsNumeric;

export namespace helios::math::concepts {

    /**
     * @brief Concept to constrain types to either integral or floating point.
     *
     * This concept should be used whenever template parameter constrain a
     * specific type to be a numeric type.
     */
    template<typename T>
    concept IsNumeric = std::integral<T> || std::floating_point<T>;
}
