/**
 * @file Component.ixx
 * @brief Base class for components attached to GameObjects.
 */
module;

#include <memory>

export module helios.engine.game.Component;

export namespace helios::engine::game {

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
     * Example usage:
     * ```cpp
     * class HealthComponent : public helios::engine::game::Component {
     *     int health_ = 100;
     * public:
     *     void takeDamage(int amount) { health_ -= amount; }
     *     int health() const { return health_; }
     * };
     *
     * // Attach to a GameObject
     * auto& health = gameObject.add<HealthComponent>();
     * ```
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
         * @details An enabled component is expected to actively participate in
         * the GameObject's lifecycle, such as rendering, updating, or responding
         * to events. A disabled component is effectively ignored in these processes.
         *
         * @return true if the component is enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Checks whether the component is disabled.
         *
         * @details Inverse of isEnabled()
         *
         * @return true if the component is disabled, false otherwise.
         */
        [[nodiscard]] bool isDisabled() const noexcept {
            return !isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
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