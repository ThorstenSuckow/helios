/**
 * @file ChaseComponent.ixx
 * @brief Component for AI chase/seek behavior.
 */
module;

export module helios.engine.modules.ai.components.ChaseComponent;

import helios.engine.ecs.CloneableComponent;

import helios.core.types;
import helios.util.Guid;

export namespace helios::engine::modules::ai::components {

    /**
     * @brief Component storing chase behavior data.
     *
     * Tracks a target entity by GUID and controls the frequency
     * of direction updates via a cooldown timer. Used by ChaseSystem
     * to steer entities towards their targets.
     */
    class ChaseComponent : public helios::engine::ecs::CloneableComponent<ChaseComponent> {

        /**
         * @brief Time between target direction updates (seconds).
         */
        float cooldown_ = 0.0f;

        /**
         * @brief Current elapsed time since last update.
         */
        float cooldownTimer_ = 0.0f;

        /**
         * @brief GUID of the entity to chase.
         */
        helios::util::Guid target_{helios::core::types::no_init};

    public:

        /**
         * @brief Default constructor.
         */
        ChaseComponent() = default;

        /**
         * @brief Returns the target entity's GUID.
         *
         * @return The GUID of the chase target.
         */
        [[nodiscard]] helios::util::Guid target() const noexcept {
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
         * @param guid The GUID of the target entity.
         */
        void setTarget(const helios::util::Guid guid) noexcept {
            target_ = guid;
        }
    };


}