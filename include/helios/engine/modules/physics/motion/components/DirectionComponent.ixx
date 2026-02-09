/**
 * @file DirectionComponent.ixx
 * @brief Component for storing a normalized direction vector.
 */
module;

#include <cassert>
#include <cmath>

export module helios.engine.modules.physics.motion.components.DirectionComponent;



import helios.math;

export namespace helios::engine::modules::physics::motion::components {

    /**
     * @brief Component that holds a normalized direction vector.
     *
     * @details
     * This component is used to store a direction for various purposes, such as
     * movement direction, facing direction, or other vector-based properties.
     * It enforces normalization of the stored vector.
     */
    class  DirectionComponent {

        /**
         * @brief The stored normalized direction vector.
         */
        helios::math::vec3f direction_{};

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

        /**
         * @brief Default constructor.
         */
        DirectionComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        DirectionComponent(const DirectionComponent& other) : direction_(other.direction()) {}

        DirectionComponent& operator=(const DirectionComponent&) = default;
        DirectionComponent(DirectionComponent&&) noexcept = default;
        DirectionComponent& operator=(DirectionComponent&&) noexcept = default;

        /**
         * @brief Sets the direction vector.
         *
         * @details
         * The provided vector must be normalized. An assertion checks this in debug builds.
         *
         * @param direction The new normalized direction vector.
         */
        void setDirection(const helios::math::vec3f direction) noexcept {
            assert(direction.length() - 1.0f <= helios::math::EPSILON_LENGTH
                && "direction must be normalized");
            direction_ = direction;
        }

        /**
         * @brief Retrieves the stored direction vector.
         *
         * @return The current normalized direction vector.
         */
        [[nodiscard]] helios::math::vec3f direction() const noexcept {
            return direction_;
        }

    };
}