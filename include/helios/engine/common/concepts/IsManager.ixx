/**
 * @file IsManager.ixx
 * @brief Concept constraining types eligible for manager registration.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsManager;

import helios.engine.common.concepts.HasTag;
import helios.engine.common.tags.ManagerTag;

export namespace helios::engine::runtime::world {
    class UpdateContext;
}

export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains T to types that provide `flush(UpdateContext&)` and
     *        declare `EngineRoleTag = ManagerTag`.
     *
     * @details A type satisfies IsManager if it:
     * 1. Provides a `void flush(UpdateContext&)` method for batch processing.
     * 2. Declares `using EngineRoleTag = ManagerTag;` to explicitly opt in.
     *
     * Used by `GameWorld::registerManager<T>()` to gate registration.
     *
     * @tparam T The type to constrain.
     *
     * @see ManagerTag
     * @see HasTag
     * @see Manager
     */
    template<class T>
    concept IsManager = requires(T& t, helios::engine::runtime::world::UpdateContext& updateContext) {
        {t.flush(updateContext) } -> std::same_as<void>;
    } && HasTag<T, tags::ManagerTag>;
}
