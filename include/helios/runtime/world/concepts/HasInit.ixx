/**
 * @file HasInit.ixx
 * @brief Concept detecting an optional init(GameWorld&) method.
 */
module;

#include <concepts>

export module helios.runtime.world.concepts.HasInit;

import helios.runtime.world.GameWorldFwd;

export namespace helios::runtime::concepts {

    /**
     * @brief Detects whether T provides a `void init(GameWorld&)` method.
     *
     * @details Used by the System type-erasure wrapper to conditionally
     * forward one-time initialization calls. If a concrete system does
     * not satisfy this concept, the init step is a no-op.
     *
     * @tparam T The type to inspect.
     *
     * @see System
     */
    template<typename T>
    concept HasInit = requires(T& t, helios::runtime::world::GameWorld& gameWorld) {
        {t.init(gameWorld) } -> std::same_as<void>;
    };


}