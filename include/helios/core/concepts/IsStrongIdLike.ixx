/**
 * @file IsStrongIdLike.ixx
 * @brief Concept for types modelling a strong identifier.
 */
module;

#include <concepts>

export module helios.core.concepts.IsStrongIdLike;

import helios.core.types.TypeDefs;

using namespace helios::core::types;
export namespace helios::core::concepts {

    /**
     * @brief Constraint for types that behave as a strong, validatable identifier.
     *
     * A type satisfying `IsStrongIdLike` exposes a validity check and a raw
     * value accessor, providing the minimal interface expected by generic
     * ECS utilities that operate on typed identifiers.
     *
     * @tparam T The identifier type to check.
     *
     * @see EntityHandle
     * @see EntityRegistry
     */
    template <typename T>
    concept IsStrongIdLike = requires(T& t)
    {
        {t.isValid()}->std::same_as<bool>;
        {t.value()}->std::same_as<StrongId_t>;
    };

}