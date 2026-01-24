/**
 * @file SteeringComponent.ixx
 * @brief Component for managing entity heading and rotation physics.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>

export module helios.engine.modules.physics.motion.components.SteeringComponent;

import helios.scene.SceneNode;
import helios.util.Guid;
import helios.core.spatial.Transform;
import helios.math;
import helios.core.units.Unit;
import helios.engine.ecs.CloneableComponent;
import helios.engine.ecs.GameObject;
import helios.engine.modules.scene.components.SceneNodeComponent;


export namespace helios::engine::modules::physics::motion::components {

    /**
     * @brief Component that manages the heading (orientation) of an entity.
     *
     * @details
     * This component handles the rotational physics of an entity, including
     * turning towards a target direction, rotation speed limits, and dampening.
     * It maintains the current and target rotation angles and is used by the
     * SteeringSystem to update the entity's orientation.
     */
    class SteeringComponent : public helios::engine::ecs::CloneableComponent<SteeringComponent> {

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
        bool stateChanged_ = true;

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
        helios::math::vec3f steeringInput_;

        /**
         * @brief The axis around which rotation occurs.
         *
         * @details Defaults to Z-axis for 2D top-down rotation.
         */
        helios::math::vec3f rotationAxis_ = helios::math::Z_AXISf;

        /**
         * @brief Flag for instant rotation mode.
         *
         * @details When true, the entity snaps to the target rotation instantly
         * rather than smoothly interpolating.
         */
        bool useInstantRotation_ = false;

        /**
         * @brief Flag indicating if the direction component should be updated from steering.
         *
         * @details If true, the DirectionComponent of the entity will be synchronized
         * with the steering input direction.
         */
        bool directionFromSteering_ = false;

    public:

        /**
         * @brief Default constructor.
         */
        SteeringComponent() = default;

        /**
         * @brief Constructs a SteeringComponent with specified settings.
         *
         * @param useInstantRotation If true, rotation snaps instantly to target.
         * @param directionFromSteering If true, synchronizes DirectionComponent with steering.
         */
        explicit SteeringComponent(bool useInstantRotation, bool directionFromSteering = false) :
        useInstantRotation_(useInstantRotation), directionFromSteering_(directionFromSteering) {}

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        explicit SteeringComponent(const SteeringComponent& other) :
            useInstantRotation_(other.useInstantRotation_),
            rotationSpeed_(other.rotationSpeed_),
            directionFromSteering_(other.directionFromSteering_),
            rotationSpeedThreshold_(other.rotationSpeedThreshold_),
            rotationDampening_(other.rotationDampening_),
            rotationAxis_(other.rotationAxis_) {}

        /**
         * @deprecated Use setSteeringIntent instead.
         */
        void setHeading(helios::math::vec3f direction, float turnIntensity) {
            setSteeringIntent(direction, turnIntensity);
        }

        /**
         * @brief Sets whether the direction component should be updated from steering.
         *
         * @param directionFromSteering True to enable synchronization, false to disable.
         */
        void setDirectionFromSteering(const bool directionFromSteering) noexcept {
            directionFromSteering_ = directionFromSteering;
        }

        /**
         * @brief Checks if direction synchronization is enabled.
         *
         * @return True if DirectionComponent is updated from steering, false otherwise.
         */
        bool directionFromSteering() const noexcept {
            return directionFromSteering_;
        }

        /**
         * @brief Returns whether instant rotation mode is enabled.
         *
         * @return True if rotation snaps instantly, false for smooth interpolation.
         */
        [[nodiscard]] bool useInstantRotation() const noexcept {
            return useInstantRotation_;
        }

        /**
         * @brief Sets the instant rotation mode.
         *
         * @param useInstantRotation True to snap instantly to target rotation, false for interpolation.
         */
        void setUseInstantRotation(const bool useInstantRotation)  noexcept {
            useInstantRotation_ = useInstantRotation;
        }


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
        void setSteeringIntent(helios::math::vec3f direction, float turnIntensity) {

            steeringInput_ = direction;
            turnIntensity_ = turnIntensity;
            stateChanged_ = true;

            if (turnIntensity_ <= helios::math::EPSILON_LENGTH) {
                steeringInput_ = {0.0f, 0.0f, 0.0f};
                stateChanged_ = false;
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
            return rotationAxis_;
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
        [[nodiscard]] bool stateChanged() const noexcept {
            return stateChanged_;
        }

        /**
         * @brief Returns the current steering input direction.
         *
         * @return Const reference to the 2D steering input vector.
         */
        [[nodiscard]] const helios::math::vec3f& steeringInput() const noexcept {
            return steeringInput_;
        }

        /**
         * @brief Returns the current steering intent (alias for steeringInput).
         *
         * @return Const reference to the steering input vector.
         */
        [[nodiscard]] const helios::math::vec3f& steeringIntent() const noexcept {
            return steeringInput_;
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

