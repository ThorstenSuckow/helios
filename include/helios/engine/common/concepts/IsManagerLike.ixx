/**
 * @file IsManagerLike.ixx
 * @brief Concept constraining types eligible for manager registration.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsManagerLike;

import helios.engine.common.concepts.HasTag;
import helios.engine.common.tags.ManagerRole;

import helios.runtime.world.UpdateContextFwd;


export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains T to types that provide `flush(UpdateContext&)` and
     *        declare `EngineRoleTag = ManagerRole`.
     *
     * @details A type satisfies IsManagerLike if it:
     * 1. Provides a `void flush(UpdateContext&)` method for batch processing.
     * 2. Declares `using EngineRoleTag = ManagerRole;` to explicitly opt in.
     *
     * Used by `GameWorld::registerResource<T>()` to gate registration.
     *
     * @tparam T The type to constrain.
     *
     * @see ManagerRole
     * @see HasTag
     * @see Manager
     */
    template<class T>
    concept IsManagerLike = requires(T& t, helios::runtime::world::UpdateContext& updateContext) {
        {t.flush(updateContext) } -> std::same_as<void>;
    } && HasTag<T, tags::ManagerRole>;
}
