/**
 * @file LastAttackerComponent.ixx
 * @brief Component for tracking the last entity that attacked this entity.
 */
module;

export module helios.engine.mechanics.combat.components.LastAttackerComponent;

import helios.engine.ecs.CloneableComponent;

import helios.engine.mechanics.combat.types.AttackContext;

export namespace helios::engine::mechanics::combat::components {

    /**
     * @brief Component that stores information about the last attack received.
     *
     * Used to track which entity last dealt damage to this entity. This enables
     * attribution of kills for scoring and other game logic. The attack context
     * is updated by the damage system when damage is applied.
     */
    class LastAttackerComponent : public helios::engine::ecs::CloneableComponent<LastAttackerComponent> {

    private:

        /**
         * @brief Context of the most recent attack received.
         */
        helios::engine::mechanics::combat::types::AttackContext lastAttackContext_{};


    public:


        LastAttackerComponent() = default;

        /**
         * @brief Copy constructor (creates empty state).
         *
         * @param other The component to copy from (state is not copied).
         */
        LastAttackerComponent(const LastAttackerComponent& other) {}

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