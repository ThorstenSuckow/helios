/**
 * @file IsStrongIdCollisionResolverLike.ixx
 * @brief Concept for strong ID collision detection strategies.
 */
module;

#include <concepts>

export module helios.ecs.concepts.IsStrongIdCollisionResolverLike;

import helios.core.types;

using namespace helios::core::types;
export namespace helios::ecs::concepts {

    /**
     * @brief Constraint for types that can track strong ID uniqueness.
     *
     * A type satisfying `IsStrongIdCollisionResolverLike` provides the
     * minimal interface required by `EntityRegistry` to detect and prevent
     * strong ID collisions during entity creation.
     *
     * @tparam T The lookup strategy type to check.
     *
     * @see EntityRegistry
     * @see HashedLookupStrategy
     * @see LinearLookupStrategy
     */
    template <typename T>
    concept IsStrongIdCollisionResolverLike = requires(T& t, const StrongId_t id)
    {
        {t.add(id)}->std::same_as<bool>;
        {t.remove(id)}->std::same_as<bool>;
        {t.has(id)}->std::same_as<bool>;
    };

}