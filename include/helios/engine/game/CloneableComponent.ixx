/**
 * @file CloneableComponent.ixx
 * @brief Base class for cloneable components attached to GameObjects.
 */
module;

#include <memory>

export module helios.engine.game.CloneableComponent;

import helios.engine.game.Component;

export namespace helios::engine::game {

    /**
     * @brief Interface for components that support cloning.
     *
     * @details Provides a polymorphic clone mechanism for components.
     * Implementing classes must provide a clone() method that returns
     * a deep copy of the component as a unique_ptr.
     */
    class Cloneable {

    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~Cloneable() = default;

        /**
         * @brief Creates a deep copy of this component.
         *
         * @return A unique_ptr owning the cloned component instance.
         */
        [[nodiscard]] virtual std::unique_ptr<Component> clone() const = 0;
    };

    /**
     * @brief CRTP base class for components that support cloning.
     *
     * @details CloneableComponent uses the Curiously Recurring Template Pattern (CRTP)
     * to provide automatic clone() implementation for derived component types.
     * The derived class must have a copy constructor.
     *
     * Example usage:
     * ```cpp
     * class HealthComponent : public CloneableComponent<HealthComponent> {
     *     int health_ = 100;
     * public:
     *     HealthComponent() = default;
     *     HealthComponent(const HealthComponent&) = default;
     * };
     *
     * auto clone = original.clone();
     * ```
     *
     * @tparam Derived The derived component type (CRTP pattern).
     */
    template<typename Derived>
    class CloneableComponent : public Component, public Cloneable {

    public:

        /**
         * @brief Creates a deep copy of the derived component.
         *
         * @details Uses the copy constructor of the Derived type to create
         * a new instance wrapped in a unique_ptr.
         *
         * @return A unique_ptr owning the cloned component.
         */
        [[nodiscard]] std::unique_ptr<Component> clone() const override {
            return std::make_unique<Derived>(static_cast<const Derived&>(*this));
        }

    };

}