/**
 * @file ShootComponent.ixx
 * @brief Component for handling projectile shooting mechanics.
 */
module;

#include <cassert>

export module helios.engine.mechanics.combat.components.ShootComponent;

import helios.math;

import helios.engine.ecs.GameObject;
import helios.engine.mechanics.combat.components.Aim2DComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;

import helios.engine.core.data.ComponentTypeId;

export namespace helios::engine::mechanics::combat::components {

    /**
     * @brief Component for handling projectile shooting with rate limiting.
     *
     * @details Manages the shooting mechanics for a GameObject by coordinating with
     * aim-components for direction and a projectile pool for projectile spawning. Implements
     * a cooldown timer to control fire rate.
     *
     *
     * Example usage:
     * ```cpp
     * auto shootComponent = std::make_unique<ShootComponent>();
     * gameObject.add(std::move(shootComponent));
     *
     * // Trigger shooting (typically from ShootCommand)
     * shootComponent->shoot(1.0f);
     * ```
     */
    class ShootComponent{

    protected:

        /**
         * @brief Current fire intensity (0.0 to 1.0).
         *
         * @details Set by shoot() and cleared when intensity is zero.
         * Controls whether projectiles are spawned during update().
         */
        float intensity_ = 0.0f;

        /**
         * @brief Cooldown interval between shots, in seconds.
         *
         * @details Derived from fireRate (1.0 / fireRate). Determines the minimum
         * time that must pass between consecutive projectile spawns.
         */
        float cooldownDelta_ = 0.0f;

        /**
         * @brief Accumulated time since last shot, in seconds.
         *
         * @details Accumulates delta time multiplied by intensity. When this value
         * exceeds cooldownDelta, projectiles are spawned and the timer is reduced.
         */
        float cooldownTimer_ = 0.0f;


        /**
         * @brief The projectile speed, in meters per second.
         */
        float projectileSpeed_ = 60.0f;

        /**
         * @brief Velocity of the source object in three-dimensional space.
         *
         * Used to adjust the trajectory of projectiles emitted by the source's movement.
         */
        helios::math::vec3f sourceVelocity_;


        /**
         * @brief Fire rate per second, i.e. projectiles than can be shot per second.
         */
        float fireRate_ = 1.0f;

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

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

        /**
         * @brief Constructs a new ShootComponent with default settings.
         */
        ShootComponent() = default;

        ShootComponent(const ShootComponent&) = default;
        ShootComponent& operator=(const ShootComponent&) = default;
        ShootComponent(ShootComponent&&) noexcept = default;
        ShootComponent& operator=(ShootComponent&&) noexcept = default;


        /**
         * @brief Sets the shooting intensity.
         *
         * @details Call with a non-zero value to start shooting, or zero to stop.
         * The intensity affects whether projectiles are spawned during update().
         *
         * @param intensity Fire intensity (0.0 to 1.0). Zero stops firing.
         * @param sourceVelocity The velocity of the object emitting the projectile.
         */
        void shoot(const float intensity, const helios::math::vec3f sourceVelocity) {

            if (intensity <= helios::math::EPSILON_LENGTH) {
                intensity_ = 0.0f;
                return;
            }
            sourceVelocity_ = sourceVelocity;
            intensity_ = intensity;
        }


        /**
         * @brief Returns the cooldown interval between shots.
         *
         * @return Cooldown interval in seconds (derived from 1.0 / fireRate).
         */
        [[nodiscard]] float cooldownDelta() const noexcept {
            return cooldownDelta_;
        }

        /**
         * @brief Returns the current fire intensity.
         *
         * @return The intensity value (0.0 to 1.0).
         */
        [[nodiscard]] float intensity() const noexcept {
            return intensity_;
        }

        /**
         * @brief Returns the accumulated time since last shot.
         *
         * @return Accumulated cooldown timer in seconds.
         */
        [[nodiscard]] float cooldownTimer() const noexcept {
            return cooldownTimer_;
        }

        /**
         * @brief Sets the accumulated cooldown timer.
         *
         * @param cooldown New timer value in seconds.
         */
        void setCooldownTimer(float cooldown) noexcept {
            cooldownTimer_ = cooldown;
        }

        /**
         * @brief Adds delta time to the cooldown timer.
         *
         * @param delta Time to add in seconds.
         */
        void updateCooldownTimerBy(float delta) noexcept {
            cooldownTimer_ += delta;
        }

        /**
         * @brief Returns the projectile speed.
         *
         * @return Projectile speed in meters per second.
         */
        [[nodiscard]] float projectileSpeed() const noexcept {
            return projectileSpeed_;
        }

        /**
         * @brief Sets the projectile speed.
         *
         * @param speed New projectile speed in meters per second.
         */
        void setProjectileSpeed(float speed) noexcept {
            projectileSpeed_ = speed;
        }


        /**
         * @brief Returns the source object's velocity.
         *
         * @return Velocity vector used for projectile trajectory adjustment.
         */
        [[nodiscard]] helios::math::vec3f sourceVelocity() const noexcept {
            return sourceVelocity_;
        }

        /**
         * @brief Returns the fire rate in projectiles per second.
         *
         * @return Fire rate (projectiles/second).
         */
        [[nodiscard]] float fireRate() const noexcept {
            return fireRate_;
        }

        /**
         * @brief Sets the fire rate.
         *
         * @details Updates the cooldown delta based on the new fire rate.
         *
         * @param fireRate The number of projectiles per second. Must be greater than zero.
         */
        void setFireRate(const float fireRate) noexcept {
            assert(fireRate > helios::math::EPSILON_LENGTH);

            fireRate_ = fireRate;
            cooldownDelta_ = 1.0f/fireRate_;
        }


    };


}

