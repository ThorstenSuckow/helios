/**
 * @file IsSystemLike.ixx
 * @brief Concept constraining types eligible for system registration.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsSystemLike;

import helios.engine.common.concepts.HasTag;
import helios.engine.common.tags.SystemRole;

import helios.runtime.world.UpdateContextFwd;


export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains T to types that provide `update(UpdateContext&)` and
     *        declare `EngineRoleTag = SystemRole`.
     *
     * @details A type satisfies IsSystemLike if it:
     * 1. Provides a `void update(UpdateContext&)` method for per-frame logic.
     * 2. Declares `using EngineRoleTag = SystemRole;` to explicitly opt in.
     *
     * Used by `SystemRegistry::add<T>()` to gate registration.
     *
     * @tparam T The type to constrain.
     *
     * @see SystemRole
     * @see HasTag
     * @see System
     */
    template<class T>
    concept IsSystemLike = requires(T& t, helios::runtime::world::UpdateContext& updateContext) {
        {t.update(updateContext) } -> std::same_as<void>;
    } && HasTag<T, tags::SystemRole>;
}
