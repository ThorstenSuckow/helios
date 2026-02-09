/**
 * @file LastAttackerComponent.ixx
 * @brief Component for tracking the last entity that attacked this entity.
 */
module;

export module helios.engine.mechanics.combat.components.LastAttackerComponent;



import helios.engine.mechanics.combat.types.AttackContext;

export namespace helios::engine::mechanics::combat::components {

    /**
     * @brief Component that stores information about the last attack received.
     *
     * Used to track which entity last dealt damage to this entity. This enables
     * attribution of kills for scoring and other game logic. The attack context
     * is updated by the damage system when damage is applied.
     */
    class LastAttackerComponent {

    private:

        /**
         * @brief Context of the most recent attack received.
         */
        helios::engine::mechanics::combat::types::AttackContext lastAttackContext_{};

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

        LastAttackerComponent() = default;

        /**
         * @brief Copy constructor (creates empty state).
         *
         * @param other The component to copy from (state is not copied).
         */
        LastAttackerComponent(const LastAttackerComponent& other) {}

        LastAttackerComponent& operator=(const LastAttackerComponent&) = default;
        LastAttackerComponent(LastAttackerComponent&&) noexcept = default;
        LastAttackerComponent& operator=(LastAttackerComponent&&) noexcept = default;

        /**
         * @brief Sets the last attack context.
         *
         * @param lastAttackContext The attack context to store.
         */
        void setLastAttackContext(const helios::engine::mechanics::combat::types::AttackContext lastAttackContext) noexcept {
            lastAttackContext_ = lastAttackContext;
        }

        /**
         * @brief Returns the last attack context.
         *
         * @return The stored AttackContext.
         */
        [[nodiscard]] helios::engine::mechanics::combat::types::AttackContext lastAttackContext() const noexcept {
            return lastAttackContext_;
        }

        /**
         * @brief Resets the component to its initial state.
         */
        void reset() {
            lastAttackContext_ = {};
        }

        /**
         * @brief Called when this entity is acquired from a pool.
         *
         * @details Resets the attack context to ensure no stale data.
         */
        void onAcquire() noexcept {
            reset();
        }

        /**
         * @brief Called when this entity is released back to a pool.
         *
         * @details Resets the attack context to ensure no stale data.
         */
        void onRelease() noexcept {
            reset();
        }

    };


}