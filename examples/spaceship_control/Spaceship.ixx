/**
 * @file Spaceship.ixx
 * @brief Spaceship entity with physics-based movement and rotation.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>

export module helios.examples.spaceshipControl.Spaceship;

import helios.engine.game.GameObject;
import helios.math.types;
import helios.math.utils;
import helios.math.transform;
import helios.scene.SceneNode;
import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::examples::spaceshipControl::Spaceship"
export namespace helios::examples::spaceshipControl {

    /**
     * @brief A controllable spaceship entity with smooth rotation and movement physics.
     *
     * @details The Spaceship class extends GameObject to provide arcade-style space flight
     * controls. It features smooth rotation towards an input direction with adjustable turn
     * speed, velocity-based movement with momentum and dampening and automatic deceleration
     * when input stops.
     *
     * The spaceship uses a 2D movement model in the XY plane with Z-axis rotation.
     * Movement and rotation are interpolated over time using delta-time for frame-rate
     * independent behavior.
     *
     * Physics parameters can be tuned at runtime via setter methods.
     *
     * @see helios::engine::game::GameObject
     * @see PlayerMoveCommand
     */
    class Spaceship : public helios::engine::game::GameObject {

        /**
         * @brief Logger instance for debug and diagnostic output.
         */
        static inline const helios::util::log::Logger& logger_ =
                helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

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
        float actualMovementSpeed_ = 0.0f;

        /**
         * @brief Current rotation angle in degrees.
         */
        float actualRotationAngle_ = 0;

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
        float actualRotationSpeed_ = 0.0f;

        /**
         * @brief Indicates whether input is currently being received.
         */
        bool isInputActive_ = true;

    public:

        /**
         * @brief Constructs a Spaceship attached to the given SceneNode.
         *
         * @param sceneNode The scene node representing this entity in the scene graph.
         */
        explicit Spaceship(helios::scene::SceneNode* sceneNode) : GameObject(sceneNode) {}

        /**
         * @brief Sets the movement target based on input direction and intensity.
         *
         * @param direction Normalized 2D direction vector indicating desired heading.
         * @param speedFactor Input intensity from 0.0 (idle) to 1.0 (full speed).
         *
         * @pre direction must be normalized (length approximately 1.0).
         */
        void move(const helios::math::vec2f direction, const float speedFactor) {

            
            if (speedFactor <= helios::math::EPSILON_LENGTH) {
                steeringInput_ = helios::math::vec2f{0.0f, 0.0f};
                throttle_ = 0.0f;
                isInputActive_ = false;
                return;
            }

            steeringInput_ = direction;
            throttle_ = speedFactor;

            assert(std::abs(direction.length() - 1.0f) <= 0.001f && "Unexpected direction vector - not normalized");

            logger_.debug(std::format("Moving Spaceship w/ speedFactor {0}", speedFactor));

            targetRotationAngle_ = helios::math::degrees(std::atan2(direction[1], direction[0]));
            rotationAngleDelta_ = std::fmod((targetRotationAngle_ - actualRotationAngle_) + 540.0f, 360.0f) - 180.0f;


            isInputActive_ = true;

            float turnBoost = 1.0f + 0.5f*std::clamp((abs(rotationAngleDelta_))/180.f, 0.0f, 1.0f);
            actualRotationSpeed_ = turnBoost * rotationSpeed_ * speedFactor;
            actualMovementSpeed_ = movementSpeed_ * speedFactor;

        }


        /**
         * @brief Updates spaceship physics for the current frame.
         *
         * @param deltaTime Time elapsed since the last frame, in seconds.
         *
         * @pre deltaTime must be non-negative.
         *
         * @details Performs the following each frame:
         *   1. Applies rotation dampening when no input is active
         *   2. Interpolates rotation towards target angle
         *   3. Applies movement dampening when no input is active
         *   4. Updates position based on current direction and speed
         *
         * @note Skips processing if deltaTime is zero to avoid division issues.
         */
        void update(const float deltaTime) override {

            logger_.debug(std::format("Updating at {0}", deltaTime));

            assert(deltaTime >= 0 && "Unexpected negative value for deltaTime");

            if (deltaTime == 0) {
                logger_.debug("deltaTime was 0, skipping update()");
                return;
            }

            // ROTATION
            if (!isInputActive_) {
                actualRotationSpeed_ *= std::pow(rotationDampening_, deltaTime);;
                if (actualRotationSpeed_ < rotationSpeedThreshold_) {
                    actualRotationSpeed_ = 0.0f;
                }
            }

            if (actualRotationSpeed_ > 0.0f) {
                float rotationStep = std::copysign(
                 std::min(std::abs(rotationAngleDelta_), actualRotationSpeed_ * deltaTime),
                 rotationAngleDelta_
                );
                actualRotationAngle_ += rotationStep;

                rotationAngleDelta_ = std::fmod((targetRotationAngle_ - actualRotationAngle_) + 540.0f, 360.0f) - 180.0f;

                if (std::abs(rotationAngleDelta_) <= 0.5f) {
                    actualRotationAngle_ = targetRotationAngle_;
                    rotationAngleDelta_  = 0.0f;
                    actualRotationSpeed_ = 0.0f;
                }

                setRotation(helios::math::rotate(
                helios::math::mat4f::identity(),
                helios::math::radians(actualRotationAngle_),
                helios::math::vec3f(0.0f, 0.0f, 1.0f)
                ));


            }

            // MOVEMENT
            // Compute the current facing direction from the actual rotation angle.
            // This converts the angle to a unit vector in the XY plane.
            const auto actualDirection_ = helios::math::vec3f(
                cos(helios::math::radians(actualRotationAngle_)), sin(helios::math::radians(actualRotationAngle_)), 0.0f
            );

            if (!isInputActive_) {
                // Apply exponential drag when no input is active.
                // This creates a smooth deceleration effect (velocity approaches zero over time).
                const float drag  = std::pow(movementDampening_, deltaTime);
                velocity_ = velocity_ * drag;
            } else {
                // Accelerate in the current facing direction.
                // Uses throttle (input intensity) to scale acceleration.
                velocity_ = velocity_ +  actualDirection_ *  (movementAcceleration_ * throttle_ * deltaTime);
            }

            // Clamp velocity to maximum speed to prevent unlimited acceleration.
            if (velocity_.length() > movementSpeed_) {
                velocity_ = velocity_.normalize() * movementSpeed_;
            }

            // Integrate velocity to update position.
            position_ = position_ + velocity_ * deltaTime;
            setTranslation(position_);

        }

        /**
         * @brief Returns the current speed as a ratio of maximum speed.
         *
         * @return A value between 0.0 (stationary) and 1.0 (maximum speed).
         */
        [[nodiscard]] float speedRatio() const noexcept override {
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
         */
        [[nodiscard]] float rotationSpeed() const noexcept { return rotationSpeed_; }

        /**
         * @brief Returns the minimum movement speed threshold.
         */
        [[nodiscard]] float movementSpeedThreshold() const noexcept { return movementSpeedThreshold_; }

        /**
         * @brief Returns the minimum rotation speed threshold.
         */
        [[nodiscard]] float rotationSpeedThreshold() const noexcept { return rotationSpeedThreshold_; }

        /**
         * @brief Returns the movement acceleration in units per second squared.
         */
        [[nodiscard]] float movementAcceleration() const noexcept { return movementAcceleration_; }

        /**
         * @brief Returns the base movement speed in units per second.
         */
        [[nodiscard]] float movementSpeed() const noexcept { return movementSpeed_; }

        /**
         * @brief Returns the rotation dampening factor.
         */
        [[nodiscard]] float rotationDampening() const noexcept { return rotationDampening_; }

        /**
         * @brief Returns the movement dampening factor.
         */
        [[nodiscard]] float movementDampening() const noexcept { return movementDampening_; }

        /**
         * @brief Returns the current rotation angle in degrees.
         */
        [[nodiscard]] float rotationAngle() const noexcept { return actualRotationAngle_; }


        // ========================================
        // Physics Parameter Setters
        // ========================================

        /**
         * @brief Sets the maximum rotation speed in degrees per second.
         *
         * @param value The new rotation speed (must be positive).
         */
        void setRotationSpeed(float value) noexcept { rotationSpeed_ = value; }

        /**
         * @brief Sets the minimum movement speed threshold.
         *
         * @param value The new threshold (0.0 to 1.0).
         */
        void setMovementSpeedThreshold(float value) noexcept { movementSpeedThreshold_ = value; }

        /**
         * @brief Sets the minimum rotation speed threshold.
         *
         * @param value The new threshold (0.0 to 1.0).
         */
        void setRotationSpeedThreshold(float value) noexcept { rotationSpeedThreshold_ = value; }

        /**
         * @brief Sets the movement acceleration in units per second squared.
         *
         * @param value The new acceleration (must be positive).
         */
        void setMovementAcceleration(float value) noexcept { movementAcceleration_ = value; }

        /**
         * @brief Sets the base movement speed in units per second.
         *
         * @param value The new movement speed (must be positive).
         */
        void setMovementSpeed(float value) noexcept { movementSpeed_ = value; }

        /**
         * @brief Sets the rotation dampening factor.
         *
         * @param value The new dampening factor (0.0 to 1.0).
         */
        void setRotationDampening(float value) noexcept { rotationDampening_ = value; }

        /**
         * @brief Sets the movement dampening factor.
         *
         * @param value The new dampening factor (0.0 to 1.0).
         */
        void setMovementDampening(float value) noexcept { movementDampening_ = value; }


        /**
         * @brief Resets all physics parameters to their default values.
         */
        void resetPhysicsToDefaults() noexcept {
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
