/**
* @file HasReset.ixx
 * @brief Concept detecting an optional reset() method.
 */
module;

#include <concepts>

export module helios.runtime.concepts.HasReset;

export namespace helios::runtime::concepts {

    /**
     * @brief Constrains types that provide a `reset()` method returning void.
     *
     * @details Used by the Manager type-erasure wrapper to conditionally
     * call `reset()` on concrete managers during level transitions or
     * game restarts.
     *
     * @tparam T The type to constrain.
     *
     * @see Manager
     * @see GameWorld::reset
     */
    template<typename T>
    concept HasReset = requires(T& t) {
        {t.reset() } -> std::same_as<void>;
    };

}


