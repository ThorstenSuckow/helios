/**
 * @file AabbColliderComponent.ixx
 * @brief Component for Axis-Aligned Bounding Box (AABB) collision detection.
 */
module;

#include <limits>

export module helios.engine.modules.physics.collision.components.AabbColliderComponent;

import helios.math.types;


export namespace helios::engine::modules::physics::collision::components {

    /**
     * @brief Component that defines an Axis-Aligned Bounding Box (AABB) for physics interactions.
     *
     * @details
     * This component attaches a spatial bounding box to a GameObject, which can be used
     * by physics systems for broad-phase collision detection or simple trigger interactions.
     */
    class AabbColliderComponent  {

        /**
         * @brief The local bounds of the collider.
         */
        helios::math::aabbf bounds_;

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
        explicit AabbColliderComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * Preserves the bounds of this component.
         */
        AabbColliderComponent(const AabbColliderComponent& other ) :
        bounds_(other.bounds_) {}

        AabbColliderComponent& operator=(const AabbColliderComponent&) = default;
        AabbColliderComponent(AabbColliderComponent&&) noexcept = default;
        AabbColliderComponent& operator=(AabbColliderComponent&&) noexcept = default;

        /**
         * @brief Resets the bounds to invalidate state when this Component is acquired.
         */
        void onAcquire() noexcept {
            bounds_ = {};
        }

        /**
         * @brief Resets the bounds to invalidate state when this Component is released.
         */
        void onRelease() noexcept {
            bounds_ = {};
        }

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
