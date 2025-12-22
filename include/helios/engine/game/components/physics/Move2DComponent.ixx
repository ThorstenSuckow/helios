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
import helios.scene.Transform;
import helios.math;
import helios.core.units;
import helios.engine.game.GameObject;
import helios.engine.game.UpdateContext;
import helios.engine.game.Updatable;
import helios.engine.game.Component;
import helios.engine.game.components.scene.SceneNodeComponent;


export namespace helios::engine::game::components::physics {

    /**
     * @brief Component for 2D physics-based movement with rotation.
     *
     * @details Handles directional movement and rotation for GameObjects in a 2D plane.
     * Implements smooth acceleration, deceleration with exponential dampening, and
     * rotation towards the input direction. Movement is applied to the associated
     * SceneNode through a SceneNodeComponent.
     *
     * The component receives input via move() which sets target direction and throttle.
     * Each frame, update() applies physics calculations to smoothly rotate towards
     * the target direction and move in the current facing direction.
     *
     * @note Requires a SceneNodeComponent to be attached to the same GameObject.
     */
    class Move2DComponent : public helios::engine::game::Updatable, public helios::engine::game::Component {

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

        /**
         * @brief Pointer to the scene node component for transform operations.
         *
         * @details Set during onAttach(). Must not be null for movement to apply.
         */
        helios::engine::game::components::scene::SceneNodeComponent* sceneNodeComponent_;

    public:

        /**
         * @brief Called when the component is attached to a GameObject.
         *
         * @details Retrieves reference to the SceneNodeComponent for transform operations.
         *
         * @param gameObject The GameObject this component is being attached to.
         */
        void onAttach(helios::engine::game::GameObject* gameObject) noexcept override {
            gameObject_ = gameObject;
            sceneNodeComponent_ = gameObject_->get<helios::engine::game::components::scene::SceneNodeComponent>();
            assert(sceneNodeComponent_ != nullptr && "Unexpected nullptr for sceneNodeComponent_");
        };

        /**
         * @brief Sets the movement direction and throttle.
         *
         * @details Updates the target rotation angle based on the input direction
         * and calculates the shortest angular distance to the target.
         *
         * @param direction Normalized 2D direction vector from analog stick.
         * @param throttle Magnitude of the stick input (0.0 to 1.0).
         */
        void move(helios::math::vec2f direction, float throttle) {
            if (throttle <= helios::math::EPSILON_LENGTH) {
                steeringInput_ = helios::math::vec2f{0.0f, 0.0f};
                throttle_ = 0.0f;
                isInputActive_ = false;
                return;
            }

            steeringInput_ = direction;
            throttle_ = throttle;

            assert(std::abs(direction.length() - 1.0f) <= 0.001f && "Unexpected direction vector - not normalized");


            targetRotationAngle_ = helios::math::degrees(std::atan2(direction[1], direction[0]));
            rotationAngleDelta_ = std::fmod((targetRotationAngle_ - actualRotationAngle_) + 540.0f, 360.0f) - 180.0f;


            isInputActive_ = true;

            float turnBoost = 1.0f + 0.5f*std::clamp((abs(rotationAngleDelta_))/180.f, 0.0f, 1.0f);
            actualRotationSpeed_ = turnBoost * rotationSpeed_ * throttle_;
            actualMovementSpeed_ = movementSpeed_ * throttle_;

        }

        /**
         * @brief Updates movement and rotation each frame.
         *
         * @details Applies rotation towards the target angle with dampening, then
         * integrates velocity to update position. Handles both active input and
         * coasting (when input stops) with exponential decay.
         *
         * @param updateContext Context containing frame delta time.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            float deltaTime = updateContext.deltaTime();

            assert(deltaTime >= 0 && "Unexpected negative value for deltaTime");



            if (deltaTime == 0) {
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

                if (sceneNodeComponent_) {
                    sceneNodeComponent_->setRotation(helios::math::rotate(
                    helios::math::mat4f::identity(),
                    helios::math::radians(actualRotationAngle_),
                    helios::math::vec3f(0.0f, 0.0f, 1.0f)
                    ));
                }


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

                if (drag <= helios::math::EPSILON_LENGTH) {
                    velocity_ = helios::math::vec3f{0.0f, 0.0f, 0.0f};
                }

            } else {
                // Accelerate in the current facing direction.
                // Uses throttle (input intensity) to scale acceleration.
                velocity_ = velocity_ +  actualDirection_ *  (movementAcceleration_ * throttle_ * deltaTime);
            }

            // Clamp velocity to maximum speed to prevent unlimited acceleration.
            if (velocity_.length() > movementSpeed_) {
                velocity_ = velocity_.normalize() * movementSpeed_;
            }


            if (sceneNodeComponent_) {
                // Integrate velocity to update position.
                sceneNodeComponent_->setTranslation(sceneNodeComponent_->translation() + velocity_ * deltaTime);
            }




        };

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

        /**
         * @brief Returns the current rotation angle in degrees.
         *
         * @return Current facing angle in degrees.
         */
        [[nodiscard]] float rotationAngle() const noexcept { return actualRotationAngle_; }

        /**
         * @brief Returns the current position from the SceneNodeComponent.
         *
         * @return Current world-space position, or zero vector if no SceneNodeComponent.
         */
        [[nodiscard]] helios::math::vec3f position() const noexcept {
            return sceneNodeComponent_ ? sceneNodeComponent_->translation() : helios::math::vec3f{0.0f, 0.0f, 0.0f};
        }

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