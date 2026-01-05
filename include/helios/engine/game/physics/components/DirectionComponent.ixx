/**
 * @file DirectionComponent.ixx
 * @brief Component for storing a normalized direction vector.
 */
module;

#include <cassert>
#include <cmath>

export module helios.engine.game.physics.components.DirectionComponent;

import helios.engine.game.Component;

import helios.math;

export namespace helios::engine::game::physics::components {

    /**
     * @brief Component that holds a normalized direction vector.
     *
     * @details
     * This component is used to store a direction for various purposes, such as
     * movement direction, facing direction, or other vector-based properties.
     * It enforces normalization of the stored vector.
     */
    class  DirectionComponent : public Component {

        /**
         * @brief The stored normalized direction vector.
         */
        helios::math::vec3f direction_{};

    public:

        /**
         * @brief Sets the direction vector.
         *
         * @details
         * The provided vector must be normalized. An assertion checks this in debug builds.
         *
         * @param direction The new normalized direction vector.
         */
        void setDirection(const helios::math::vec3f direction) noexcept {
            assert(std::abs(1.0f - direction.length()) <= helios::math::EPSILON_LENGTH
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