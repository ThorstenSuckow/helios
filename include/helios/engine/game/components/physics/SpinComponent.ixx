/**
 * @file SpinComponent.ixx
 * @brief Component for applying continuous spin rotation to an entity.
 */
module;


export module helios.engine.game.components.physics.SpinComponent;

import helios.engine.game.Component;

import helios.math;

export namespace helios::engine::game::components::physics {

    /**
     * @brief Component that defines spin properties for an entity.
     *
     * @details
     * This component is used by the SpinSystem to apply continuous rotation
     * around a specific axis. It stores the rotation speed, axis, and current
     * rotation state.
     */
    class SpinComponent : public Component {

        /**
         * @brief Rotation speed in degrees per second.
         */
        float rotationSpeed_ = 720.0f;

        /**
         * @brief The axis around which the entity spins.
         */
        helios::math::vec3f rotationAxis_ = helios::math::Z_AXISf;

        /**
         * @brief Initial spin angle offset in degrees.
         */
        float spinAngle_ = 45.0f;

        /**
         * @brief Current accumulated rotation angle in degrees.
         */
        float currentRotation_ = 0.0f;

    public:

        /**
         * @brief Constructs a SpinComponent.
         *
         * @param rotationSpeed Speed of rotation in degrees per second.
         * @param rotationAxis The axis to rotate around.
         */
        explicit SpinComponent(
            const float rotationSpeed, const helios::math::vec3f rotationAxis
        ) : rotationSpeed_(rotationSpeed),
            rotationAxis_(rotationAxis) {}

        /**
         * @brief Returns the rotation axis.
         *
         * @return The normalized rotation axis vector.
         */
        [[nodiscard]] helios::math::vec3f axis() {
            return rotationAxis_;
        }

        /**
         * @brief Returns the rotation speed.
         *
         * @return Speed in degrees per second.
         */
        [[nodiscard]] float speed() const {
            return rotationSpeed_;
        }

        /**
         * @brief Returns the current accumulated rotation.
         *
         * @return Current rotation angle in degrees.
         */
        [[nodiscard]] float rotation() const {
            return currentRotation_;
        }

        /**
         * @brief Returns the initial spin angle offset.
         *
         * @return Initial angle in degrees.
         */
        [[nodiscard]] float spinAngle() const {
            return spinAngle_;
        }

        /**
         * @brief Sets the current accumulated rotation.
         *
         * @param currentRotation The new rotation angle in degrees.
         */
        void setRotation(float currentRotation) {
            currentRotation_ = currentRotation;
        }
    };

}