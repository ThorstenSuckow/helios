/**
 * @file Traits.ixx
 * @brief Compile-time traits for ECS component lifecycle hooks.
 */
module;

#include <concepts>

export module helios.engine.ecs.Traits;


export namespace helios::engine::ecs::traits {

    /**
     * @brief Trait for components that support removal interception.
     *
     * When a type `T` satisfies `HasOnRemove`, the `SparseSet` will invoke
     * `onRemove()` before removing the element. If the method returns `false`,
     * removal is cancelled.
     *
     * ## Usage
     *
     * ```cpp
     * struct MyComponent {
     *     bool onRemove() {
     *         // Cleanup logic...
     *         return true; // Allow removal
     *     }
     * };
     *
     * static_assert(traits::HasOnRemove<MyComponent>);
     * ```
     *
     * @tparam T The type to check.
     *
     * @see SparseSet::remove
     */
    template<typename T>
    concept HasOnRemove = requires(T t) {
        {t.onRemove()} -> std::convertible_to<bool>;
    };


    /**
     * @brief Called when this GameObject is acquired from a pool.
     *
     * @details Notifies all attached components by calling their `onAcquire()` method.
     * Used by the pool system to reset or initialize component state before reuse.
     *
     * @see GameObjectPool
     * @see Component::onAcquire
     */
    template<typename T>
    concept HasOnAcquire = requires(T t) {
        {t.onAcquire()} -> std::same_as<void>;
    };


    /**
     * @brief Called when this GameObject is released back to a pool.
     *
     * @details Notifies all attached components by calling their `onRelease()` method.
     * Used by the pool system to clean up component state before pooling.
     *
     * @see GameObjectPool
     * @see Component::onRelease
     */
    template<typename T>
    concept HasOnRelease = requires(T t) {
        {t.onRelease()} -> std::same_as<void>;
    };

}