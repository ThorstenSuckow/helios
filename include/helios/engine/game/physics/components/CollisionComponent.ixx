/**
 * @file CollisionComponent.ixx
 * @brief Component for layer-based collision detection and event generation.
 */
module;

#include <iostream>
#include <format>

export module helios.engine.game.physics.components.CollisionComponent;

import helios.engine.game.Component;
import helios.engine.game.GameObject;

export namespace helios::engine::game::physics::components {

    /**
     * @class CollisionComponent
     * @brief Defines collision layer membership and interaction masks for a GameObject.
     *
     * @details CollisionComponent enables layer-based collision filtering by assigning
     * each entity to a specific layer and defining which other layers it can interact with.
     * Two separate bitmasks control trigger (non-physical) or solid (physical) collision
     * generation.
     *
     * Trigger collisions are asymmetric: entity A can detect entity B without B detecting A.
     * Solid collisions should be treated symmetrically by the collision system.
     *
     * The collision reporter flag controls whether this entity can emit collision events.
     * Non-reporter entities participate in collision detection but do not generate events.
     *
     * Example usage:
     * ```cpp
     * constexpr uint32_t LAYER_PLAYER = 1 << 0;
     * constexpr uint32_t LAYER_ENEMY  = 1 << 1;
     * constexpr uint32_t LAYER_BULLET = 1 << 2;
     *
     * auto& collision = gameObject.add<CollisionComponent>(LAYER_PLAYER);
     * collision.setTriggerCollisionMask(LAYER_ENEMY | LAYER_BULLET);
     * collision.setSolidCollisionMask(LAYER_ENEMY);
     * ```
     */
    class CollisionComponent : public Component {

        /**
         * @brief The collision layer this component belongs to.
         */
        const uint32_t layerId_{};

        /**
         * @brief Bitmask specifying layers that generate trigger collisions.
         *
         * @details Trigger collision masks may be asymmetric, i.e. a component B must not
         * know about the existence of another layerId.
         */
        uint32_t triggerCollisionMask_{};

        /**
         * @brief Bitmask specifying layers that generate solid collisions.
         *
         * @details Solid collision should be symmetric and in the calling API treated as such:
         * `(maskB & thisLayerId) != 0 && (thisMask & layerBId) != 0`
         */
        uint32_t solidCollisionMask_{};

        /**
         * @brief Flag indicating whether this component can report collision events.
         */
        bool isCollisionReporter_ = true;

    public:

        /**
         * @brief Constructs a CollisionComponent with a layer ID.
         *
         * @param layerId The collision layer this entity belongs to.
         * @param isCollisionReporter Whether this entity can emit collision events.
         */
        explicit CollisionComponent(uint32_t layerId, bool isCollisionReporter = true) :
            layerId_(layerId),
            isCollisionReporter_(isCollisionReporter) {}

        /**
         * @brief Returns whether this component can emit collision events.
         *
         * @return True if collision events can be generated, false otherwise.
         */
        [[nodiscard]] bool isCollisionReporter() const noexcept {
            return isCollisionReporter_;
        }

        /**
         * @brief Sets the solid collision mask.
         *
         * @param solidCollisionMask Bitmask of layers to generate solid collisions with.
         *
         * @return Reference to this component for method chaining.
         */
        CollisionComponent& setSolidCollisionMask(const uint32_t solidCollisionMask) noexcept {
            solidCollisionMask_ = solidCollisionMask;
            return *this;
        }

        /**
         * @brief Sets the trigger collision mask.
         *
         * @param triggerCollisionMask Bitmask of layers to generate trigger collisions with.
         *
         * @return Reference to this component for method chaining.
         */
        CollisionComponent& setTriggerCollisionMask(const uint32_t triggerCollisionMask) noexcept {
            triggerCollisionMask_ = triggerCollisionMask;
            return *this;
        }

        /**
         * @brief Returns the solid collision mask.
         *
         * @return Bitmask of layers this entity generates solid collisions with.
         */
        [[nodiscard]] uint32_t solidCollisionMask() const noexcept {
            return solidCollisionMask_;
        }

        /**
         * @brief Returns the trigger collision mask.
         *
         * @return Bitmask of layers this entity generates trigger collisions with.
         */
        [[nodiscard]] uint32_t triggerCollisionMask() const noexcept {
            return triggerCollisionMask_;
        }

        /**
         * @brief Returns the collision layer ID.
         *
         * @return The layer this entity belongs to.
         */
        [[nodiscard]] unsigned int layerId() const noexcept {
            return layerId_;
        }

    };


}