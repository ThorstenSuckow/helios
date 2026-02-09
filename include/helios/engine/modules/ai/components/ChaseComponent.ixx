/**
 * @file ChaseComponent.ixx
 * @brief Component for AI chase/seek behavior.
 */
module;

#include <cassert>
export module helios.engine.modules.ai.components.ChaseComponent;



import helios.core.types;
import helios.util.Guid;

import helios.engine.ecs.EntityHandle;


export namespace helios::engine::modules::ai::components {

    /**
     * @brief Component storing chase behavior data.
     *
     * Tracks a target entity by handle and controls the frequency
     * of direction updates via a cooldown timer. Used by ChaseSystem
     * to steer entities towards their targets.
     */
    class ChaseComponent  {

        /**
         * @brief Time between target direction updates (seconds).
         */
        float cooldown_ = 0.0f;

        /**
         * @brief Current elapsed time since last update.
         */
        float cooldownTimer_ = 0.0f;

        /**
         * @brief EntityHandle of the entity to chase.
         */
        helios::engine::ecs::EntityHandle target_{};

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
         * @brief Default constructor.
         */
        ChaseComponent() = default;

        ChaseComponent(const ChaseComponent&) = default;
        ChaseComponent& operator=(const ChaseComponent&) = default;
        ChaseComponent(ChaseComponent&&) noexcept = default;
        ChaseComponent& operator=(ChaseComponent&&) noexcept = default;

        /**
         * @brief Returns the target entity's handle.
         *
         * @return The handle of the chase target.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle target() const noexcept {
            return target_;
        }

        /**
         * @brief Returns the cooldown duration.
         *
         * @return Time between direction updates in seconds.
         */
        [[nodiscard]] float cooldown() const noexcept {
            return cooldown_;
        }

        /**
         * @brief Sets the cooldown duration.
         *
         * @param cooldown Time between direction updates in seconds.
         */
        void setCooldown(const float cooldown) noexcept {
            cooldown_ = cooldown;
        }

        /**
         * @brief Returns the current cooldown timer value.
         *
         * @return Elapsed time since last direction update.
         */
        [[nodiscard]] float cooldownTimer() const noexcept {
            return cooldownTimer_;
        }

        /**
         * @brief Advances the cooldown timer by delta time.
         *
         * @param delta Time to add to the timer.
         */
        void updateCooldownTimerBy(const float delta) noexcept {
            cooldownTimer_ += delta;
        }

        /**
         * @brief Sets the cooldown timer to a specific value.
         *
         * @param timer New timer value.
         */
        void setCooldownTimer(const float timer) noexcept {
            cooldownTimer_ = timer;
        }

        /**
         * @brief Sets the target entity to chase.
         *
         * @param entityHandle The EntityHandle of the target entity.
         */
        void setTarget(const helios::engine::ecs::EntityHandle& entityHandle) noexcept {
            assert(entityHandle.isValid() && "Unexpected invalid entityHandle");
            target_ = entityHandle;
        }
    };


}