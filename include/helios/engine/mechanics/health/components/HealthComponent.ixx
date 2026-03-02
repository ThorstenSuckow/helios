/**
 * @file HealthComponent.ixx
 * @brief Component for tracking entity health.
 */
module;

#include <algorithm>
#include <cassert>

export module helios.engine.mechanics.health.components.HealthComponent;


import helios.engine.mechanics.health.types;

using namespace helios::engine::mechanics::health::types;

export namespace helios::engine::mechanics::health::components {


    /**
     * @brief Component for tracking entity health and damage.
     *
     * Manages current and maximum health values for an entity. Provides
     * methods for taking damage, healing, and checking alive status.
     */
    class HealthComponent  {

    private:

        /**
         * @brief Maximum health value.
         */
        float maxHealth_{};


        /**
         * @brief Current health value.
         */
        float health_{};

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;



        /**
         * @brief Flags controlling the response when health reaches zero.
         */
        HealthDepletedBehavior healthDepletedBehavior_ = HealthDepletedBehavior::None;

        /**
         * @brief True if the health value was modified since the last clear.
         */
        bool isDirty_ = true;

    public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
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
        HealthComponent(const HealthComponent& other)
        :
        maxHealth_(other.maxHealth_),
        healthDepletedBehavior_(other.healthDepletedBehavior_)
        {
            assert(maxHealth_ != 0.0f && "max health must not be 0");
            isDirty_ = true;
        }

        HealthComponent& operator=(const HealthComponent&) = default;
        HealthComponent(HealthComponent&&) noexcept = default;
        HealthComponent& operator=(HealthComponent&&) noexcept = default;

        /**
         * @brief Sets the health depletion response behaviour.
         *
         * @param behavior Bitmask of HealthDepletedBehavior flags.
         */
        void setHealthDepletedBehavior(const HealthDepletedBehavior behavior) noexcept {
            healthDepletedBehavior_ = behavior;
        }

        /**
         * @brief Returns the health depletion response behaviour.
         *
         * @return Current HealthDepletedBehavior flags.
         */
        [[nodiscard]] HealthDepletedBehavior healthDepletedBehavior() const noexcept {
            return healthDepletedBehavior_;
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
            isDirty_ = true;
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
            isDirty_ = true;
        }

        /**
         * @brief Sets the maximum health value.
         *
         * @param maxHealth The new maximum health.
         */
        void setMaxHealth(const float maxHealth) noexcept {
            maxHealth_ = maxHealth;
            isDirty_ = true;
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
         * @brief Checks whether the health value has been modified.
         *
         * @return True if modified since the last clearDirty() call.
         */
        [[nodiscard]] bool isDirty() const noexcept {
            return isDirty_;
        }

        /**
         * @brief Resets the dirty flag.
         */
        void clearDirty() noexcept {
            isDirty_ = false;
        }


        /**
         * @brief Resets health to maximum.
         */
        void reset() {
            health_ = maxHealth_;
            isDirty_ = true;
        }

        /**
         * @brief Called when this entity is acquired from a pool.
         *
         * @details Resets health to maximum value.
         */
        void onAcquire() noexcept {
            reset();
        }

        /**
         * @brief Called when this entity is released back to a pool.
         *
         * @details Resets health to maximum value.
         */
        void onRelease() noexcept {
            reset();
        }




    };


}