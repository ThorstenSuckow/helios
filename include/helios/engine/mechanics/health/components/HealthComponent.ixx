/**
 * @file HealthComponent.ixx
 * @brief Component for tracking entity health.
 */
module;

#include <algorithm>
#include <cassert>

export module helios.engine.mechanics.health.components.HealthComponent;

import helios.engine.ecs.CloneableComponent;


export namespace helios::engine::mechanics::health::components {


    /**
     * @brief Component for tracking entity health and damage.
     *
     * Manages current and maximum health values for an entity. Provides
     * methods for taking damage, healing, and checking alive status.
     */
    class HealthComponent : public helios::engine::ecs::CloneableComponent<HealthComponent> {

    private:

        /**
         * @brief Maximum health value.
         */
        float maxHealth_{};


        /**
         * @brief Current health value.
         */
        float health_{};

    public:


        HealthComponent() = default;

        /**
         * @brief Constructs a HealthComponent with the given maximum health.
         *
         * @param maxHealth The maximum health value.
         */
        explicit HealthComponent(const float maxHealth) : maxHealth_(maxHealth) {};

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        HealthComponent(const HealthComponent& other) : maxHealth_(other.maxHealth_) {
            assert(maxHealth_ != 0.0f && "max health must not be 0");

        }

        /**
         * @brief Applies damage to this entity.
         *
         * Health is clamped to a minimum of 0.
         *
         * @param damage The amount of damage to apply.
         */
        void takeDamage(const float damage) noexcept {
            health_ = std::max(0.0f, health_ - damage);
        }

        /**
         * @brief Heals this entity.
         *
         * Health is clamped to the maximum health value.
         *
         * @param amount The amount to heal.
         */
        void heal(const float amount) noexcept {
            health_ = std::min(maxHealth_, health_ + amount);
        }

        /**
         * @brief Sets the maximum health value.
         *
         * @param maxHealth The new maximum health.
         */
        void setMaxHealth(const float maxHealth) noexcept {
            maxHealth_ = maxHealth;
        }

        /**
         * @brief Checks if the entity is alive.
         *
         * @return True if health is greater than 0.
         */
        [[nodiscard]] bool isAlive() const noexcept {
            return health_ > 0.0f;
        }

        /**
         * @brief Returns the current health value.
         *
         * @return The current health.
         */
        [[nodiscard]] float health() const noexcept {
            return health_;
        }

        /**
         * @brief Returns the maximum health value.
         *
         * @return The maximum health.
         */
        [[nodiscard]] float maxHealth() const noexcept {
            return maxHealth_;
        }

        /**
         * @brief Resets health to maximum.
         */
        void reset() {
            health_ = maxHealth_;
        }

        /**
         * @copydoc CloneableComponent::onAcquire()
         */
        void onAcquire() noexcept override {
            reset();
        }

        /**
         * @copydoc CloneableComponent::onRelease()
         */
        void onRelease() noexcept override {
            reset();
        }




    };


}