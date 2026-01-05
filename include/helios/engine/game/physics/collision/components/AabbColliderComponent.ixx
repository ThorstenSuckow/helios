/**
 * @file AabbColliderComponent.ixx
 * @brief Component for Axis-Aligned Bounding Box (AABB) collision detection.
 */
module;

#include <limits>

export module helios.engine.game.physics.collision.components.AabbColliderComponent;

import helios.math.types;
import helios.engine.game.Component;

export namespace helios::engine::game::physics::collision::components {

    /**
     * @brief Component that defines an Axis-Aligned Bounding Box (AABB) for physics interactions.
     *
     * @details
     * This component attaches a spatial bounding box to a GameObject, which can be used
     * by physics systems for broad-phase collision detection or simple trigger interactions.
     */
    class AabbColliderComponent : public helios::engine::game::Component {

        /**
         * @brief The local bounds of the collider.
         */
        helios::math::aabbf bounds_;

    public:

        /**
         * @brief Default constructor.
         */
        explicit AabbColliderComponent() = default;


        /**
         * @brief Retrieves the bounds of the collider.
         *
         * @return A const reference to the AABB.
         */
        [[nodiscard]] const helios::math::aabbf& bounds() {
            return bounds_;
        }

        /**
         * @brief Sets the bounds of the collider.
         *
         * @param bounds The new AABB to set.
         */
        void setBounds(const helios::math::aabbf& bounds) {
            bounds_ = bounds;
        }

        /**
         * @brief Checks whether the bounds of the collider have been initialized.
         *
         * Bounds of the collider component are considered to be initialized
         * if the minimum X coordinate is less or equal to the maximum X coordinate.
         *
         * @return true if the bounds are considered to be initialized; otherwise, false.
         */
        [[nodiscard]] bool boundsInitialized() const noexcept {
            return bounds_.min()[0] <= bounds_.max()[0];
        }
    };

}
