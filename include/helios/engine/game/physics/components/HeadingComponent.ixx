/**
 * @file HeadingComponent.ixx
 * @brief Component for managing entity heading and rotation physics.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>

export module helios.engine.game.physics.components.HeadingComponent;

import helios.scene.SceneNode;
import helios.util.Guid;
import helios.core.Transform;
import helios.math;
import helios.core.units;
import helios.engine.game.Component;
import helios.engine.game.GameObject;
import helios.engine.game.components.scene.SceneNodeComponent;


export namespace helios::engine::game::physics::components {

    /**
     * @brief Component that manages the heading (orientation) of an entity.
     *
     * @details
     * This component handles the rotational physics of an entity, including
     * turning towards a target direction, rotation speed limits, and dampening.
     * It maintains the current and target rotation angles and is used by the
     * HeadingSystem to update the entity's orientation.
     */
    class HeadingComponent : public helios::engine::game::Component {

    protected:

        /**
         * @brief Default maximum rotation speed in degrees per second.
         */
        static constexpr float DEFAULT_ROTATION_SPEED = 560.0f;

        /**
         * @brief Default minimum rotation speed before rotation stops completely.
         */
        static constexpr float DEFAULT_ROTATION_SPEED_THRESHOLD = 0.1f;

        /**
         * @brief Default exponential decay factor for rotation when input stops.
         */
        static constexpr float DEFAULT_ROTATION_DAMPENING = 0.0001f;

        /**
         * @brief Maximum rotation speed in degrees per second.
         */
        float rotationSpeed_ = DEFAULT_ROTATION_SPEED;

        /**
         * @brief Minimum rotation speed before rotation stops completely.
         */
        float rotationSpeedThreshold_ = DEFAULT_ROTATION_SPEED_THRESHOLD;

        /**
         * @brief Exponential decay factor for rotation when input stops.
         */
        float rotationDampening_ = DEFAULT_ROTATION_DAMPENING;

        /**
         * @brief Current rotation angle in degrees.
         */
        float currentRotationAngle_ = 0;

        /**
         * @brief Target rotation angle derived from input direction.
         */
        float targetRotationAngle_  = 0;

        /**
         * @brief Shortest angular distance to target rotation.
         */
        float rotationAngleDelta_ = 0;

        /**
         * @brief Current rotation speed after applying input and dampening.
         */
        float currentRotationSpeed_ = 0.0f;

        /**
         * @brief Indicates whether input is currently being received.
         */
        bool isInputActive_ = true;

        /**
         * @brief Intensity of the turn input, typically from 0.0 to 1.0.
         *
         * @details Derived from the magnitude of the input direction vector.
         * Used to modulate rotation speed based on stick deflection.
         */
        float turnIntensity_ = 0.0f;

        /**
         * @brief Current steering input as 2D direction vector.
         */
        helios::math::vec3f headingInput_;


    public:

        /**
         * @brief Sets the heading direction and turn intensity from input.
         *
         * @details Updates the heading input vector and turn intensity. If the
         * turn intensity is below the epsilon threshold, input is deactivated
         * and the heading is reset to zero.
         *
         * @param direction Normalized direction vector representing the desired heading.
         * @param turnIntensity Magnitude of the input (0.0 to 1.0).
         *
         * @pre direction must be a normalized vector (length ~= 1.0).
         */
        void setHeading(helios::math::vec3f direction, float turnIntensity) {

            headingInput_ = direction;
            turnIntensity_ = turnIntensity;
            isInputActive_ = true;

            if (turnIntensity_ <= helios::math::EPSILON_LENGTH) {
                headingInput_ = {0.0f, 0.0f, 0.0f};
                isInputActive_ = false;
                turnIntensity_ = 0.0f;
                return;
            }

            assert(std::abs(direction.length() - 1.0f) <= 0.001f && "Unexpected direction vector - not normalized");
        }

        /**
         * @brief Returns the axis around which rotation occurs.
         *
         * @details For 2D gameplay on the XY plane, this is typically the Z-axis.
         *
         * @return The rotation axis as a unit vector.
         */
        [[nodiscard]] helios::math::vec3f rotationAxis() const noexcept {
            return helios::math::Z_AXISf;
        }

        /**
         * @brief Sets the current rotation speed.
         *
         * @param speed The new rotation speed.
         */
        void setCurrentRotationSpeed(float speed) noexcept {
            currentRotationSpeed_ = speed;
        }

        /**
         * @brief Returns the current rotation speed.
         *
         * @return The current rotation speed.
         */
        [[nodiscard]] float currentRotationSpeed() const noexcept {
            return currentRotationSpeed_;
        }

        /**
         * @brief Returns the angular distance to the target rotation.
         *
         * @return The angle delta in degrees.
         */
        [[nodiscard]] float rotationAngleDelta() const noexcept {
            return rotationAngleDelta_;
        }
        
        /**
         * @brief Returns the current rotation angle.
         *
         * @return The current rotation angle in degrees.
         */
        [[nodiscard]] float currentRotationAngle() const noexcept {
            return currentRotationAngle_;
        }

        /**
         * @brief Returns the target rotation angle.
         *
         * @return The target rotation angle in degrees.
         */
        [[nodiscard]] float targetRotationAngle() const noexcept {
            return targetRotationAngle_;
        }

        /**
         * @brief Sets the target rotation angle.
         *
         * @param angle The new target rotation angle in degrees.
         */
        void setTargetRotationAngle(float angle) noexcept {
            targetRotationAngle_ = angle;
        }

        /**
         * @brief Sets the current rotation angle.
         *
         * @param angle The new rotation angle in degrees.
         */
        void setCurrentRotationAngle(float angle) noexcept {
            currentRotationAngle_ = angle;
        }

        /**
         * @brief Sets the angular distance to the target rotation.
         *
         * @param delta The new angle delta in degrees.
         */
        void setRotationAngleDelta(float delta) noexcept {
            rotationAngleDelta_ = delta;
        }
        
        /**
         * @brief Checks if input is currently active.
         *
         * @return True if input is active, false otherwise.
         */
        [[nodiscard]] bool isInputActive() const noexcept {
            return isInputActive_;
        }

        /**
         * @brief Returns the current steering input direction.
         *
         * @return Const reference to the 2D steering input vector.
         */
        [[nodiscard]] const helios::math::vec3f& headingInput() const noexcept {
            return headingInput_;
        }

        /**
         * @brief Returns the maximum rotation speed in degrees per second.
         *
         * @return Maximum rotation speed value.
         */
        [[nodiscard]] float rotationSpeed() const noexcept { return rotationSpeed_; }

        /**
         * @brief Returns the minimum rotation speed threshold.
         *
         * @return Rotation speed threshold below which rotation stops.
         */
        [[nodiscard]] float rotationSpeedThreshold() const noexcept { return rotationSpeedThreshold_; }

        /**
         * @brief Returns the rotation dampening factor.
         *
         * @return Exponential decay factor for rotation.
         */
        [[nodiscard]] float rotationDampening() const noexcept { return rotationDampening_; }

        /**
         * @brief Sets the maximum rotation speed in degrees per second.
         *
         * @param value New rotation speed value.
         */
        void setRotationSpeed(float value) noexcept { rotationSpeed_ = value; }



        /**
         * @brief Sets the minimum rotation speed threshold.
         *
         * @param value New threshold value.
         */
        void setRotationSpeedThreshold(float value) noexcept {
            rotationSpeedThreshold_ = value;
        }



        /**
         * @brief Sets the rotation dampening factor.
         *
         * @param value New dampening factor (exponential decay).
         */
        void setRotationDampening(float value) noexcept {
            rotationDampening_ = value;
        }

        /**
         * @brief Returns the current turn intensity.
         *
         * @return Turn intensity value, typically from 0.0 to 1.0.
         */
        [[nodiscard]] float turnIntensity() const noexcept {
            return turnIntensity_;
        }

        // ========================================
        // Reset to Defaults
        // ========================================

        /**
         * @brief Resets all physics parameters to their default values.
         */
        void resetToDefaults() noexcept {
            rotationSpeed_ = DEFAULT_ROTATION_SPEED;
            rotationSpeedThreshold_ = DEFAULT_ROTATION_SPEED_THRESHOLD;
            rotationDampening_ = DEFAULT_ROTATION_DAMPENING;
        }
    };


}

