/**
 * @file LastDamageComponent.ixx
 * @brief Component for tracking the last entity that attacked this entity.
 */
module;

export module helios.gameplay.damage.components.LastDamageComponent;



import helios.engine.common.types.DamageContext;

using namespace helios::engine::common::types;

export namespace helios::gameplay::damage::components {

    /**
     * @brief Component that stores information about the last damage received.
     *
     * Used to track which entity last dealt damage to this entity. This enables
     * attribution of kills for scoring and other game logic. The damage context
     * is updated by the damage system when damage is applied.
     */
    template<typename THandle>
    class LastDamageComponent {

    private:

        /**
         * @brief Context of the most recent damage received.
         */
        helios::engine::common::types::DamageContext<THandle> damageContext_{};

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

        LastDamageComponent() = default;

        /**
         * @brief Copy constructor (creates empty state).
         *
         * @param other The component to copy from (state is not copied).
         */
        LastDamageComponent(const LastDamageComponent<THandle>& other) = default;

        LastDamageComponent& operator=(const LastDamageComponent<THandle>&) = default;
        LastDamageComponent(LastDamageComponent<THandle>&&) noexcept = default;
        LastDamageComponent& operator=(LastDamageComponent<THandle>&&) noexcept = default;

        /**
         * @brief Sets the last damage context.
         *
         * @param damageContext The damage context to store.
         */
        void setDamageContext(const DamageContext<THandle>& damageContext) noexcept {
            damageContext_ = damageContext;
        }

        /**
         * @brief Returns the last damage context.
         *
         * @return The stored AttackContext.
         */
        [[nodiscard]] DamageContext<THandle> damageContext() const noexcept {
            return damageContext_;
        }

        /**
         * @brief Resets the component to its initial state.
         */
        void reset() {
            damageContext_ = {};
        }

        /**
         * @brief Called when this entity is acquired from a pool.
         *
         * @details Resets the damage context to ensure no stale data.
         */
        void onAcquire() noexcept {
            reset();
        }

        /**
         * @brief Called when this entity is released back to a pool.
         *
         * @details Resets the damage context to ensure no stale data.
         */
        void onRelease() noexcept {
            reset();
        }

    };


}