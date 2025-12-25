/**
 * @file AabbColliderComponent.ixx
 * @brief Component for Axis-Aligned Bounding Box (AABB) collision detection.
 */
module;

export module helios.engine.game.components.physics.AabbColliderComponent;

import helios.math.types;
import helios.engine.game.Component;

export namespace helios::engine::game::components::physics {

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

    };

}
