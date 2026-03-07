/**
 * @file HasInit.ixx
 * @brief Concept detecting an optional init(GameWorld&) method.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.HasInit;

export namespace helios::engine::runtime::world {
    class GameWorld;
}

export namespace helios::engine::common::concepts {

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
    concept HasInit = requires(T& t, helios::engine::runtime::world::GameWorld& gameWorld) {
        {t.init(gameWorld) } -> std::same_as<void>;
    };


}