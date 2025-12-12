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

import helios.game.GameObject;
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
     * speed, velocity-based movement with momentum and dampening, deadzone filtering for
     * analog stick input, and automatic deceleration when input stops.
     *
     * The spaceship uses a 2D movement model in the XY plane with Z-axis rotation.
     * Movement and rotation are interpolated over time using delta-time for frame-rate
     * independent behavior.
     *
     * @see helios::game::GameObject
     * @see PlayerMoveCommand
     */
    class Spaceship : public helios::game::GameObject {

        /**
         * @brief Logger instance for debug and diagnostic output.
         */
        static inline const helios::util::log::Logger& logger_ =
                helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Maximum rotation speed in degrees per second.
         */
        static constexpr float BASE_ROTATION_SPEED = 560.0f;

        /**
         * @brief Minimum movement speed before the ship stops completely.
         */
        static constexpr float MOVEMENT_SPEED_THRESHOLD = 0.1f;

        /**
         * @brief Minimum rotation speed before rotation stops completely.
         */
        static constexpr float ROTATIONS_SPEED_THRESHOLD = 0.1f;

        /**
         * @brief MOVEMENT_ACCELERATION
         */
        static constexpr float MOVEMENT_ACCELERATION = 30.0f;

        /**
         * @brief Base movement speed in units per second.
         */
        static constexpr float BASE_MOVEMENT_SPEED = 30.0f;

        /**
         * @brief Exponential decay factor for rotation when input stops.
         */
        static constexpr float ROTATION_DAMPENING = 0.0001f;

        /**
         * @brief Exponential decay factor for movement when input stops.
         */
        static constexpr float MOVEMENT_DAMPENING = 0.1f;

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
         * @brief Base rotation speed multiplier.
         */
        float baseRotationSpeed = 16.0f;

        /**
         * @brief Current rotation speed after applying input and dampening.
         */
        float actualRotationSpeed_ = 0.0f;

        /**
         * @brief Analog stick deadzone threshold (0.0 to 1.0).
         */
        float deadzone = 0.4f;


        /**
         * @brief Current world position of the spaceship.
         */
        helios::math::vec3f actualPosition_ = helios::math::vec3f(0.0f, 0.0f, 0.0f);

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
         *
         * @note Input below the deadzone threshold is ignored, causing the ship
         *       to coast with gradual deceleration.
         *
         * @todo Deadzone handling should be moved to the InputManager.
         */
        void move(const helios::math::vec2f direction, const float speedFactor) {

            /**
             * @todo normalization needs to happen with the input manager
             */
            if (speedFactor <= deadzone) {
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
            actualRotationSpeed_ = turnBoost * BASE_ROTATION_SPEED * speedFactor;
            actualMovementSpeed_ = BASE_MOVEMENT_SPEED * speedFactor;

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
                actualRotationSpeed_ *= std::pow(ROTATION_DAMPENING, deltaTime);;
                if (actualRotationSpeed_ < ROTATIONS_SPEED_THRESHOLD) {
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

                rotate(helios::math::rotate(
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
                const float drag  = std::pow(0.1f, deltaTime);
                velocity_ = velocity_ * drag;
            } else {
                // Accelerate in the current facing direction.
                // Uses throttle (input intensity) to scale acceleration.
                velocity_ = velocity_ +  actualDirection_ *  (MOVEMENT_ACCELERATION * throttle_ * deltaTime);
            }

            // Clamp velocity to maximum speed to prevent unlimited acceleration.
            if (velocity_.length() > BASE_MOVEMENT_SPEED) {
                velocity_ = velocity_.normalize() * BASE_MOVEMENT_SPEED;
            }

            // Integrate velocity to update position.
            actualPosition_ = actualPosition_ + velocity_ * deltaTime;
            translate(actualPosition_);

        }

        /**
         * @brief Returns the current speed as a ratio of maximum speed.
         *
         * @return A value between 0.0 (stationary) and 1.0 (maximum speed).
         */
        [[nodiscard]] float speedRatio() const noexcept override {
            return velocity_.length() / BASE_MOVEMENT_SPEED;
        }

    };

}
