/**
 * @file Move2DComponent.ixx
 * @brief Component for 2D physics-based movement .
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>

export module helios.engine.game.physics.motion.components.Move2DComponent;

import helios.scene.SceneNode;
import helios.util.Guid;
import helios.core.spatial.Transform;
import helios.math;
import helios.core.units.Unit;
import helios.engine.ecs.CloneableComponent;
import helios.engine.ecs.GameObject;
import helios.engine.game.scene.components.SceneNodeComponent;


export namespace helios::engine::game::physics::motion::components {

    /**
     * @brief Component for 2D physics-based movement.
     *
     * @details
     * Stores configuration and runtime state for 2D physics-based movement.
     * This component is used by the Move2DSystem to apply smooth acceleration
     * and deceleration to the entity.
     *
     * The component receives input via move() which sets target direction and throttle.
     * The actual physics simulation (integration of velocity, application of dampening)
     * is performed by the Move2DSystem.
     *
     * @note Rotation/heading is handled separately by SteeringComponent and SteeringSystem.
     *
     * @see helios::engine::game::physics::motion::components::SteeringComponent
     * @see helios::engine::game::physics::systems::Move2DSystem
     */
    class Move2DComponent : public helios::engine::ecs::CloneableComponent<Move2DComponent> {

    protected:


        // ========================================
        // Default Physics Constants
        // ========================================


        /**
         * @brief Default minimum movement speed before the ship stops completely.
         */
        static constexpr float DEFAULT_MOVEMENT_SPEED_THRESHOLD = 0.1f;


        /**
         * @brief Default movement acceleration in units per second squared.
         */
        static constexpr float DEFAULT_MOVEMENT_ACCELERATION = 30.0f;

        /**
         * @brief Default base movement speed in units per second.
         */
        static constexpr float DEFAULT_MOVEMENT_SPEED = 30.0f;


        /**
         * @brief Default exponential decay factor for movement when input stops.
         */
        static constexpr float DEFAULT_MOVEMENT_DAMPENING = 0.1f;


        // ========================================
        // Configurable Physics Parameters
        // ========================================

        /**
         * @brief Minimum movement speed before the ship stops completely.
         */
        float movementSpeedThreshold_ = DEFAULT_MOVEMENT_SPEED_THRESHOLD;

        /**
         * @brief Movement acceleration in units per second squared.
         */
        float movementAcceleration_ = DEFAULT_MOVEMENT_ACCELERATION;

        /**
         * @brief Base movement speed in units per second.
         */
        float movementSpeed_ = DEFAULT_MOVEMENT_SPEED;


        /**
         * @brief Exponential decay factor for movement when input stops.
         */
        float movementDampening_ = DEFAULT_MOVEMENT_DAMPENING;

        // ========================================
        // Runtime State Variables
        // ========================================

        /**
         * @brief Current movement speed after applying input and dampening.
         */
        float currentMovementSpeed_ = 0.0f;

        /**
         * @brief Indicates whether input is currently being received.
         */
        bool stateChanged_ = true;

        
        /**
         * @brief Current throttle value from input (0.0 to 1.0).
         */
        float throttle_ = 0.0f;

        /**
         * @brief Current steering input as 2D direction vector.
         */
        helios::math::vec2f direction_;

        /**
         * @brief Current velocity vector in world space.
         */
        helios::math::vec3f velocity_;

        /**
         * @brief Inherited velocity from parent or external forces.
         *
         * @details Used for momentum inheritance when detaching from parent objects.
         */
        helios::math::vec3f inheritedVelocity_{0.0f};

        /**
         * @brief Flag for instant acceleration mode.
         *
         * @details When true, the entity accelerates instantly to target speed
         * rather than smoothly ramping up.
         */
        bool useInstantAcceleration_ = false;

    public:

        /**
         * @brief Default constructor with default physics parameters.
         */
        Move2DComponent() = default;

        /**
         * @brief Constructs a Move2DComponent with a specified movement speed.
         *
         * @param movementSpeed The maximum movement speed in units per second.
         * @param movementAcceleration The acceleration rate (default: DEFAULT_MOVEMENT_ACCELERATION).
         */
        explicit Move2DComponent(
            float movementSpeed, 
            float movementAcceleration = DEFAULT_MOVEMENT_ACCELERATION
            
        ) :
        movementSpeed_(movementSpeed), movementAcceleration_(movementAcceleration) {}

        /**
         * @brief Constructs a Move2DComponent with instant acceleration mode.
         *
         * @param movementSpeed The maximum movement speed in units per second.
         * @param useInstantAcceleration If true, acceleration is instant.
         */
        explicit Move2DComponent(
            float movementSpeed,
            bool useInstantAcceleration

        ) :
        movementSpeed_(movementSpeed), useInstantAcceleration_(useInstantAcceleration) {}

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        explicit Move2DComponent(const Move2DComponent& other) :
        useInstantAcceleration_(other.useInstantAcceleration_),
        movementSpeed_(other.movementSpeed_),
        movementSpeedThreshold_(other.movementSpeedThreshold_),
        movementAcceleration_ (other.movementAcceleration_),
        movementDampening_ (other.movementDampening_)
        {}


        /**
         * @brief Sets the movement direction and throttle.
         *
         * @param direction Normalized 3D direction vector.
         * @param throttle Magnitude of the stick input (0.0 to 1.0).
         * 
         * @deprecated use setMoveIntent
         */
        void move(helios::math::vec3f direction, float throttle) {
            setMoveIntent(direction, throttle);
        }
        
        /**
         * @brief Sets the movement direction and throttle.
         *
         * @param direction Normalized 3D direction vector.
         * @param throttle Magnitude of the stick input (0.0 to 1.0).
         */
        void setMoveIntent(helios::math::vec3f direction, float throttle) {
            
            if (throttle <= helios::math::EPSILON_LENGTH) {
                direction_ = helios::math::vec2f{0.0f, 0.0f};
                throttle_ = 0.0f;
                stateChanged_ = false;
                return;
            }

            direction_ = direction.toVec2();
            throttle_ = throttle;

            assert(std::abs(direction.length() - 1.0f) <= helios::math::EPSILON_LENGTH && "Unexpected direction vector - not normalized");

            stateChanged_ = true;

            currentMovementSpeed_ = movementSpeed_ * throttle_;
        }

        /**
         * @brief Returns whether instant acceleration mode is enabled.
         *
         * @return True if acceleration is instant, false for smooth ramping.
         */
        [[nodiscard]] bool useInstantAcceleration() const noexcept {
            return useInstantAcceleration_;
        }

        /**
         * @brief Sets the inherited velocity from parent or external forces.
         *
         * @param inheritedVelocity The velocity vector to inherit.
         */
        void setInheritedVelocity(helios::math::vec3f inheritedVelocity) {
            inheritedVelocity_ = inheritedVelocity;
        }

        /**
         * @brief Returns the inherited velocity vector.
         *
         * @return The inherited velocity from parent or external forces.
         */
        [[nodiscard]] helios::math::vec3f inheritedVelocity() const noexcept {
            return inheritedVelocity_;
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
         * @brief Returns the current movement speed.
         *
         * @return The current movement speed.
         */
        [[nodiscard]] float currentMovementSpeed() const noexcept {
            return currentMovementSpeed_;
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
        [[nodiscard]] bool stateChanged() const noexcept {
            return stateChanged_;
        }

        /**
         * @brief Returns the current steering input direction.
         *
         * @return Const reference to the 2D steering input vector.
         */
        [[nodiscard]] const helios::math::vec2f& direction() const noexcept {
            return direction_;
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
         * @brief Returns the minimum movement speed threshold.
         *
         * @return Movement speed threshold below which movement stops.
         */
        [[nodiscard]] float movementSpeedThreshold() const noexcept { return movementSpeedThreshold_; }

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
         * @brief Returns the movement dampening factor.
         *
         * @return Exponential decay factor for movement.
         */
        [[nodiscard]] float movementDampening() const noexcept { return movementDampening_; }


        // ========================================
        // Physics Parameter Setters
        // ========================================

        /**
         * @brief Sets the minimum movement speed threshold.
         *
         * @param value New threshold value.
         */
        void setMovementSpeedThreshold(float value) noexcept { movementSpeedThreshold_ = value; }


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
            movementSpeedThreshold_ = DEFAULT_MOVEMENT_SPEED_THRESHOLD;
            movementAcceleration_ = DEFAULT_MOVEMENT_ACCELERATION;
            movementSpeed_ = DEFAULT_MOVEMENT_SPEED;
            movementDampening_ = DEFAULT_MOVEMENT_DAMPENING;
        }
    };


}

