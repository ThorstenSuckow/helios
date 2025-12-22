/**
 * @file Aim2DComponent.ixx
 * @brief Component for handling 2D aiming direction.
 */
module;

export module helios.engine.game.components.gameplay.Aim2DComponent;

import helios.math.types;
import helios.engine.game.UpdateContext;
import helios.engine.game.Updatable;
import helios.engine.game.Component;


export namespace helios::engine::game::components::gameplay {

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
    class Aim2DComponent : public helios::engine::game::Component {

    protected:

        /**
         * @brief Current aim direction as normalized 2D vector.
         */
        helios::math::vec2f direction_{0.0f, 0.0f};

        /**
         * @brief Current frequency factor (0.0 to 1.0) for additional fire rate control.
         */
        float frequency_ = 0.0f;

    public:

        /**
         * @brief Sets the aim direction and fire frequency.
         *
         * @param direction Normalized 2D direction vector.
         * @param freq Fire frequency factor (0.0 to 1.0).
         */
        virtual void aim(helios::math::vec2f direction, float freq) {
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