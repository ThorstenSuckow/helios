/**
 * @file Component.ixx
 * @brief Base class for components attached to GameObjects.
 */
module;

#include <memory>

export module helios.engine.ecs.Component;

export namespace helios::engine::ecs {

    class GameObject;

    /**
     * @brief Abstract base class for components attached to GameObjects.
     *
     * @details Components encapsulate specific behaviors or data that can be attached
     * to a GameObject. Each component maintains a back-reference to its owning
     * GameObject, established via onAttach().
     *
     * Derived classes implement specific functionality such as physics, rendering,
     * or input handling. Components can optionally implement the Updatable interface
     * if they require per-frame updates.
     *
     * ## Enabled State
     *
     * Each component has an `isEnabled()` flag that controls whether it participates
     * in system processing. This allows fine-grained control over individual behaviors
     * without removing components from the GameObject.
     *
     * - `isEnabled() == true` — Component is active and processed by systems
     * - `isEnabled() == false` — Component is skipped by systems but remains attached
     *
     * Systems should check `isEnabled()` before processing a component:
     *
     * ```cpp
     * for (auto [obj, collision] : gameWorld.find<CollisionComponent>().each()) {
     *     if (collision.isDisabled()) {
     *         continue;  // Skip disabled components
     *     }
     *     // Process enabled component...
     * }
     * ```
     *
     * @note Disabling a component does **not** disable the entire GameObject.
     *       Use `GameObject::setActive(false)` to exclude an entity from all processing.
     *
     * Example usage:
     * ```cpp
     * class HealthComponent : public helios::engine::ecs::Component {
     *     int health_ = 100;
     * public:
     *     void takeDamage(int amount) { health_ -= amount; }
     *     int health() const { return health_; }
     * };
     *
     * // Attach to a GameObject
     * auto& health = gameObject.add<HealthComponent>();
     *
     * // Temporarily disable collision
     * gameObject.get<CollisionComponent>()->disable();
     * ```
     *
     * @see GameObject
     * @see CloneableComponent
     */
    class Component {

    protected:
        /**
         * @brief Non-owning pointer to the owning GameObject.
         *
         * @details Set by onAttach() when the component is added to a GameObject.
         * nullptr until attached.
         */
        GameObject* gameObject_ = nullptr;

        /**
         * @brief Boolean flag indicating whether this component is enabled.
         */
        bool isEnabled_ = true;


    public:
        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~Component() = default;

        /**
         * @brief Default constructor.
         */
        Component() = default;

        /**
         * @brief Called when the component is attached to a GameObject.
         *
         * @param gameObject Pointer to the owning GameObject.
         *
         * @note Derived classes that override this method should call the base
         *       implementation to ensure gameObject_ is set correctly.
         */
        virtual void onAttach(GameObject* gameObject) noexcept {
            gameObject_ = gameObject;
        };

        /**
         * @brief Called when the component's GameObject is re-acquired from a pool.
         *
         * @details Override this method to reinitialize component state when the
         * owning GameObject is recycled from an object pool. This allows components
         * to reset their internal state for reuse without requiring a full destruction
         * and reconstruction cycle.
         *
         * The default implementation does nothing.
         */
        virtual void onAcquire() noexcept {
            // noop
        };

        /**
         * @brief Called when the component's GameObject is released to a pool.
         *
         * @details Override this method to prune any component state when the owning GameObject
         * is released to an object pool. This allows for reducing memory footprints
         * of GameObjects not actively participating in the GameWorld.
         *
         * The default implementation does nothing.
         */
        virtual void onRelease() noexcept {
            // noop
        };

        /**
         * @brief Checks whether this component is attached to a GameObject.
         *
         * @return True if the component has an owning GameObject, false otherwise.
         */
        [[nodiscard]] bool isAttached() const noexcept {
            return gameObject_ != nullptr;
        }

        /**
         * @brief Checks whether the component is enabled.
         *
         * @details An enabled component is processed by systems during the game loop.
         * A disabled component remains attached to the GameObject but is skipped
         * by systems that check this flag.
         *
         * Use this for temporary deactivation of specific behaviors without removing
         * the component entirely (e.g., temporary invulnerability, disabled collision).
         *
         * @return True if the component is enabled, false otherwise.
         *
         * @see disable()
         * @see enable()
         * @see isDisabled()
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Checks whether the component is disabled.
         *
         * @details Convenience method, equivalent to `!isEnabled()`.
         *
         * @return True if the component is disabled, false otherwise.
         *
         * @see isEnabled()
         */
        [[nodiscard]] bool isDisabled() const noexcept {
            return !isEnabled_;
        }

        /**
         * @brief Enables this component.
         *
         * @details An enabled component will be processed by systems that check
         * the enabled state. This is the default state for newly created components.
         *
         * @see disable()
         * @see isEnabled()
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         *
         * @details A disabled component remains attached to the GameObject but
         * should be skipped by systems. This allows temporary deactivation of
         * specific behaviors without modifying the component configuration.
         *
         * Example use cases:
         * - Temporary invulnerability (disable CollisionComponent)
         * - Frozen movement (disable Move2DComponent)
         * - Stealth mode (disable detection-related components)
         *
         * @note This does **not** call any lifecycle callbacks. The component
         *       remains fully attached and can be re-enabled at any time.
         *
         * @see enable()
         * @see isDisabled()
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        /**
         * @brief Called when the owning GameObject becomes active.
         *
         * @details Override this method to perform initialization or state restoration
         * when the GameObject is activated. The default implementation does nothing.
         */
        virtual void onActivate() noexcept {

        };

        /**
         * @brief Called when the owning GameObject becomes inactive.
         *
         * @details Override this method to perform cleanup or state preservation
         * when the GameObject is deactivated. The default implementation does nothing.
         */
        virtual void onDeactivate() noexcept {

        };

        /**
         * @brief Returns the owning GameObject.
         *
         * @return Non-owning pointer to the GameObject, or nullptr if not attached.
         */
        [[nodiscard]] GameObject* gameObject() const noexcept {
            return gameObject_;
        }
    };

}