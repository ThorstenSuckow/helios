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

    /**
     * @brief Trait for components that can be enabled/disabled.
     *
     * @details When a type `T` satisfies `HasToggleable`, the component can be
     * toggled on/off via `enable()` and `disable()` methods. Both methods must
     * be present for the trait to be satisfied.
     *
     * ## Usage
     *
     * ```cpp
     * struct MyComponent {
     *     bool isEnabled_ = true;
     *
     *     void enable() noexcept { isEnabled_ = true; }
     *     void disable() noexcept { isEnabled_ = false; }
     * };
     *
     * static_assert(traits::HasToggleable<MyComponent>);
     * ```
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept HasToggleable = requires(T t) {
        {t.disable()} -> std::same_as<void>;
        {t.enable()} -> std::same_as<void>;
    };

    /**
     * @brief Trait for components with post-copy initialization.
     *
     * @details When a type `T` satisfies `HasClone`, the `onClone()` method is
     * called after copy construction during entity cloning. Use this for
     * initialization that requires the copy to be complete.
     *
     * ## Usage
     *
     * ```cpp
     * struct SceneNodeComponent {
     *     SceneNode* sceneNode_;
     *
     *     void onClone(const SceneNodeComponent& source) {
     *         // Create a new SceneNode for this clone
     *         auto node = std::make_unique<SceneNode>(source.sceneNode_->shareRenderable());
     *         sceneNode_ = source.sceneNode_->parent()->addNode(std::move(node));
     *     }
     * };
     *
     * static_assert(traits::HasClone<SceneNodeComponent>);
     * ```
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept HasClone = requires(T t, const T& source) {
        {t.onClone(source)} -> std::same_as<void>;
    };

    /**
     * @brief Trait for components responding to GameObject activation.
     *
     * @details When a type `T` satisfies `HasActivatable`, the `onActivate()`
     * and `onDeactivate()` methods are called when the owning GameObject's
     * active state changes via `setActive()`. Both methods must be present.
     *
     * ## Usage
     *
     * ```cpp
     * struct AIComponent {
     *     void onActivate() { startBehaviorTree(); }
     *     void onDeactivate() { pauseBehaviorTree(); }
     * };
     *
     * static_assert(traits::HasActivatable<AIComponent>);
     * ```
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept HasActivatable = requires(T t) {
        {t.onActivate()} -> std::same_as<void>;
        {t.onDeactivate()} -> std::same_as<void>;
    };


}