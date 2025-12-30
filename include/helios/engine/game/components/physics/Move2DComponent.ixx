/**
 * @file Move2DComponent.ixx
 * @brief Component for 2D physics-based movement with rotation.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>

export module helios.engine.game.components.physics.Move2DComponent;

import helios.scene.SceneNode;
import helios.util.Guid;
import helios.core.Transform;
import helios.math;
import helios.core.units;
import helios.engine.game.Component;
import helios.engine.game.GameObject;
import helios.engine.game.components.scene.SceneNodeComponent;


export namespace helios::engine::game::components::physics {

    /**
     * @brief Component for 2D physics-based movement with rotation.
     *
     * @details
     * Stores configuration and runtime state for 2D physics-based movement.
     * This component is used by the movement system to apply smooth acceleration,
     * deceleration, and rotation to the entity.
     *
     * The component receives input via move() which sets target direction and throttle.
     * The actual physics simulation (integration of velocity, application of dampening)
     * is performed by the Movement2DSystem.
     */
    class Move2DComponent : public helios::engine::game::Component {

    protected:


        // ========================================
        // Default Physics Constants
        // ========================================

        /**
         * @brief Default maximum rotation speed in degrees per second.
         */
        static constexpr float DEFAULT_ROTATION_SPEED = 560.0f;

        /**
         * @brief Default minimum movement speed before the ship stops completely.
         */
        static constexpr float DEFAULT_MOVEMENT_SPEED_THRESHOLD = 0.1f;

        /**
         * @brief Default minimum rotation speed before rotation stops completely.
         */
        static constexpr float DEFAULT_ROTATION_SPEED_THRESHOLD = 0.1f;

        /**
         * @brief Default movement acceleration in units per second squared.
         */
        static constexpr float DEFAULT_MOVEMENT_ACCELERATION = 30.0f;

        /**
         * @brief Default base movement speed in units per second.
         */
        static constexpr float DEFAULT_MOVEMENT_SPEED = 30.0f;

        /**
         * @brief Default exponential decay factor for rotation when input stops.
         */
        static constexpr float DEFAULT_ROTATION_DAMPENING = 0.0001f;

        /**
         * @brief Default exponential decay factor for movement when input stops.
         */
        static constexpr float DEFAULT_MOVEMENT_DAMPENING = 0.1f;

        /**
         * @brief Default base rotation speed multiplier.
         */
        static constexpr float DEFAULT_BASE_ROTATION_SPEED_MULTIPLIER = 16.0f;

        // ========================================
        // Configurable Physics Parameters
        // ========================================

        /**
         * @brief Maximum rotation speed in degrees per second.
         */
        float rotationSpeed_ = DEFAULT_ROTATION_SPEED;

        /**
         * @brief Minimum movement speed before the ship stops completely.
         */
        float movementSpeedThreshold_ = DEFAULT_MOVEMENT_SPEED_THRESHOLD;

        /**
         * @brief Minimum rotation speed before rotation stops completely.
         */
        float rotationSpeedThreshold_ = DEFAULT_ROTATION_SPEED_THRESHOLD;

        /**
         * @brief Movement acceleration in units per second squared.
         */
        float movementAcceleration_ = DEFAULT_MOVEMENT_ACCELERATION;

        /**
         * @brief Base movement speed in units per second.
         */
        float movementSpeed_ = DEFAULT_MOVEMENT_SPEED;

        /**
         * @brief Exponential decay factor for rotation when input stops.
         */
        float rotationDampening_ = DEFAULT_ROTATION_DAMPENING;

        /**
         * @brief Exponential decay factor for movement when input stops.
         */
        float movementDampening_ = DEFAULT_MOVEMENT_DAMPENING;

        /**
         * @brief Base rotation speed multiplier.
         */
        float baseRotationSpeedMultiplier_ = DEFAULT_BASE_ROTATION_SPEED_MULTIPLIER;

        // ========================================
        // Runtime State Variables
        // ========================================

        /**
         * @brief Current movement speed after applying input and dampening.
         */
        float currentMovementSpeed_ = 0.0f;

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
         * @brief Current throttle value from input (0.0 to 1.0).
         */
        float throttle_ = 0.0f;

        /**
         * @brief Current steering input as 2D direction vector.
         */
        helios::math::vec2f steeringInput_;

        /**
         * @brief Current velocity vector in world space.
         */
        helios::math::vec3f velocity_;


    public:

        /**
         * @brief Default constructor with default physics parameters.
         */
        Move2DComponent() = default;

        /**
         * @brief Constructs a Move2DComponent with a specified movement speed.
         *
         * @param movementSpeed The maximum movement speed in units per second.
         */
        explicit Move2DComponent(float movementSpeed) : movementSpeed_(movementSpeed) {}


        /**
         * @brief Sets the movement direction and throttle.
         *
         * @details Updates the target rotation angle based on the input direction
         * and calculates the shortest angular distance to the target.
         *
         * @param direction Normalized 2D direction vector from analog stick.
         * @param throttle Magnitude of the stick input (0.0 to 1.0).
         */
        void move(helios::math::vec3f direction, float throttle) {
            if (throttle <= helios::math::EPSILON_LENGTH) {
                steeringInput_ = helios::math::vec2f{0.0f, 0.0f};
                throttle_ = 0.0f;
                isInputActive_ = false;
                return;
            }

            steeringInput_ = direction.toVec2();
            throttle_ = throttle;

            assert(std::abs(direction.length() - 1.0f) <= 0.001f && "Unexpected direction vector - not normalized");
            

            isInputActive_ = true;

            currentMovementSpeed_ = movementSpeed_ * throttle_;

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
         * @brief Sets the current velocity vector.
         *
         * @param velocity The new velocity vector.
         */
        void setVelocity(helios::math::vec3f velocity) noexcept {
            velocity_ = velocity;
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
         * @brief Returns the current movement speed.
         *
         * @return The current movement speed.
         */
        [[nodiscard]] float currentMovementSpeed() const noexcept {
            return currentMovementSpeed_;
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
         * @brief Sets the current movement speed.
         *
         * @param speed The new movement speed.
         */
        void setCurrentMovementSpeed(float speed) noexcept {
            currentMovementSpeed_ = speed;
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
        [[nodiscard]] const helios::math::vec2f& steeringInput() const noexcept {
            return steeringInput_;
        }

        /**
         * @brief Returns the current throttle value (0.0 to 1.0).
         *
         * @return Current throttle magnitude from input.
         */
        [[nodiscard]] float throttle() const noexcept {
            return throttle_;
        }

        /**
         * @brief Returns the current velocity vector.
         *
         * @return Const reference to the 3D velocity vector in world space.
         */
        [[nodiscard]] const helios::math::vec3f& velocity() const noexcept {
            return velocity_;
        }

        /**
         * @brief Returns the current speed as a ratio of maximum speed.
         *
         * @return A value between 0.0 (stationary) and 1.0 (maximum speed).
         */
        [[nodiscard]] float speedRatio() const noexcept {
            // Prevent division by zero if movementSpeed_ is zero or very close to zero
            if (std::abs(movementSpeed_) < helios::math::EPSILON_LENGTH) {
                return 0.0f;
            }
            return velocity_.length() / movementSpeed_;
        }

        // ========================================
        // Physics Parameter Getters
        // ========================================

        /**
         * @brief Returns the maximum rotation speed in degrees per second.
         *
         * @return Maximum rotation speed value.
         */
        [[nodiscard]] float rotationSpeed() const noexcept { return rotationSpeed_; }

        /**
         * @brief Returns the minimum movement speed threshold.
         *
         * @return Movement speed threshold below which movement stops.
         */
        [[nodiscard]] float movementSpeedThreshold() const noexcept { return movementSpeedThreshold_; }

        /**
         * @brief Returns the minimum rotation speed threshold.
         *
         * @return Rotation speed threshold below which rotation stops.
         */
        [[nodiscard]] float rotationSpeedThreshold() const noexcept { return rotationSpeedThreshold_; }

        /**
         * @brief Returns the movement acceleration in units per second squared.
         *
         * @return Movement acceleration value.
         */
        [[nodiscard]] float movementAcceleration() const noexcept { return movementAcceleration_; }

        /**
         * @brief Returns the maximum movement speed in units per second.
         *
         * @return Maximum movement speed value.
         */
        [[nodiscard]] float movementSpeed() const noexcept { return movementSpeed_; }

        /**
         * @brief Returns the rotation dampening factor.
         *
         * @return Exponential decay factor for rotation.
         */
        [[nodiscard]] float rotationDampening() const noexcept { return rotationDampening_; }

        /**
         * @brief Returns the movement dampening factor.
         *
         * @return Exponential decay factor for movement.
         */
        [[nodiscard]] float movementDampening() const noexcept { return movementDampening_; }


        // ========================================
        // Physics Parameter Setters
        // ========================================

        /**
         * @brief Sets the maximum rotation speed in degrees per second.
         *
         * @param value New rotation speed value.
         */
        void setRotationSpeed(float value) noexcept { rotationSpeed_ = value; }

        /**
         * @brief Sets the minimum movement speed threshold.
         *
         * @param value New threshold value.
         */
        void setMovementSpeedThreshold(float value) noexcept { movementSpeedThreshold_ = value; }

        /**
         * @brief Sets the minimum rotation speed threshold.
         *
         * @param value New threshold value.
         */
        void setRotationSpeedThreshold(float value) noexcept { rotationSpeedThreshold_ = value; }

        /**
         * @brief Sets the movement acceleration in units per second squared.
         *
         * @param value New acceleration value.
         */
        void setMovementAcceleration(float value) noexcept { movementAcceleration_ = value; }

        /**
         * @brief Sets the maximum movement speed in units per second.
         *
         * @param value New maximum speed value.
         */
        void setMovementSpeed(float value) noexcept { movementSpeed_ = value; }

        /**
         * @brief Sets the rotation dampening factor.
         *
         * @param value New dampening factor (exponential decay).
         */
        void setRotationDampening(float value) noexcept { rotationDampening_ = value; }

        /**
         * @brief Sets the movement dampening factor.
         *
         * @param value New dampening factor (exponential decay).
         */
        void setMovementDampening(float value) noexcept { movementDampening_ = value; }

        // ========================================
        // Reset to Defaults
        // ========================================

        /**
         * @brief Resets all physics parameters to their default values.
         */
        void resetToDefaults() noexcept {
            rotationSpeed_ = DEFAULT_ROTATION_SPEED;
            movementSpeedThreshold_ = DEFAULT_MOVEMENT_SPEED_THRESHOLD;
            rotationSpeedThreshold_ = DEFAULT_ROTATION_SPEED_THRESHOLD;
            movementAcceleration_ = DEFAULT_MOVEMENT_ACCELERATION;
            movementSpeed_ = DEFAULT_MOVEMENT_SPEED;
            rotationDampening_ = DEFAULT_ROTATION_DAMPENING;
            movementDampening_ = DEFAULT_MOVEMENT_DAMPENING;
        }
    };


}

