/**
* @file HasClear.ixx
 * @brief Concept detecting an optional clear() method.
 */
module;

#include <concepts>

export module helios.runtime.concepts.HasClear;

export namespace helios::runtime::concepts {

    /**
     * @brief Constrains types that provide a noexcept-less `clear()` method returning void.
     *
     * @details Used by IsCommandBuffer to verify that a concrete command buffer
     * supports discarding all queued commands without executing them.
     *
     * @tparam T The type to constrain.
     *
     * @see IsCommandBuffer
     * @see CommandBuffer
     */
    template<typename T>
    concept HasClear = requires(T& t) {
        {t.clear() } -> std::same_as<void>;
    };

}


