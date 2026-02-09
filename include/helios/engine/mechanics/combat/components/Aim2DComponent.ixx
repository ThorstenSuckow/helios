/**
 * @file Aim2DComponent.ixx
 * @brief Component for handling 2D aiming direction.
 */
module;

export module helios.engine.mechanics.combat.components.Aim2DComponent;

import helios.math.types;
import helios.engine.runtime.world.UpdateContext;


import helios.math;

import helios.engine.core.data.ComponentTypeId;

export namespace helios::engine::mechanics::combat::components {

    /**
     * @brief Component for handling 2D aiming direction and fire frequency.
     *
     * @details Receives aim direction and frequency factor from Aim2DCommand.
     * Derived classes can override to implement specific aiming behaviors such
     * as cannon rotation or crosshair positioning.
     *
     * @note This is a base implementation with empty methods. Derive from this
     *       class to implement game-specific aiming logic.
     */
    class Aim2DComponent {

    protected:

        /**
         * @brief Current aim direction as normalized 2D vector.
         */
        helios::math::vec2f direction_{0.0f, 0.0f};

        /**
         * @brief Current frequency factor (0.0 to 1.0) for additional fire rate control.
         */
        float frequency_ = 0.0f;

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

        Aim2DComponent() = default;
        Aim2DComponent(const Aim2DComponent&) = default;
        Aim2DComponent& operator=(const Aim2DComponent&) = default;
        Aim2DComponent(Aim2DComponent&&) noexcept = default;
        Aim2DComponent& operator=(Aim2DComponent&&) noexcept = default;

        /**
         * @brief Sets the aim direction and fire frequency.
         *
         * @param direction Normalized 2D direction vector.
         * @param freq Fire frequency factor (0.0 to 1.0).
         */
        void aim(helios::math::vec2f direction, float freq) {
            if (freq <= helios::math::EPSILON_LENGTH) {
                direction_ = {};
                frequency_ = 0.0f;
            }
            direction_ = direction;
            frequency_ = freq;
        }


        /**
         * @brief Returns the current aim direction.
         *
         * @return Const reference to the normalized 2D direction vector.
         */
        [[nodiscard]] const helios::math::vec2f& direction() const noexcept {
            return direction_;
        }

        /**
         * @brief Returns the current frequency factor.
         *
         * @return Fire frequency factor in range [0.0, 1.0].
         */
        [[nodiscard]] float frequency() const noexcept {
            return frequency_;
        }


    };


}