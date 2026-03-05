/**
 * @file IsSystem.ixx
 * @brief Concept constraining types eligible for system registration.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsSystem;

import helios.engine.common.concepts.HasTag;
import helios.engine.common.tags.SystemTag;

export namespace helios::engine::runtime::world {
    class UpdateContext;
}

export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains T to types that provide `update(UpdateContext&)` and
     *        declare `EngineRoleTag = SystemTag`.
     *
     * @details A type satisfies IsSystem if it:
     * 1. Provides a `void update(UpdateContext&)` method for per-frame logic.
     * 2. Declares `using EngineRoleTag = SystemTag;` to explicitly opt in.
     *
     * Used by `SystemRegistry::add<T>()` to gate registration.
     *
     * @tparam T The type to constrain.
     *
     * @see SystemTag
     * @see HasTag
     * @see System
     */
    template<class T>
    concept IsSystem = requires(T& t, helios::engine::runtime::world::UpdateContext& updateContext) {
        {t.update(updateContext) } -> std::same_as<void>;
    } && HasTag<T, tags::SystemTag>;
}
