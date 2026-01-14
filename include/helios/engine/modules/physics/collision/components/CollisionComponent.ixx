/**
 * @file CollisionComponent.ixx
 * @brief Component for layer-based collision detection and event generation.
 */
module;

#include <array>
#include <format>

export module helios.engine.modules.physics.collision.components.CollisionComponent;

import helios.engine.ecs.CloneableComponent;
import helios.engine.ecs.GameObject;

import helios.engine.modules.physics.collision.types.CollisionBehavior;

export namespace helios::engine::modules::physics::collision::components {

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
    class CollisionComponent : public helios::engine::ecs::CloneableComponent<CollisionComponent> {

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

        static_assert(std::to_underlying(helios::engine::modules::physics::collision::types::CollisionBehavior::size_) <= 32);

        /**
         * @brief Per-behavior layer masks for solid collisions.
         *
         * @details Each array index corresponds to a CollisionBehavior enum value.
         * The bitmask at each index indicates which layers trigger that behavior
         * for solid collisions.
         */
        std::array<
            uint32_t,
            std::to_underlying(helios::engine::modules::physics::collision::types::CollisionBehavior::size_)
        > solidCollisionBehavior_{};

        /**
         * @brief Per-behavior layer masks for trigger collisions.
         *
         * @details Each array index corresponds to a CollisionBehavior enum value.
         * The bitmask at each index indicates which layers trigger that behavior
         * for trigger collisions.
         */
        std::array<
            uint32_t,
            std::to_underlying(helios::engine::modules::physics::collision::types::CollisionBehavior::size_)
        > triggerCollisionBehavior_{};

    public:

        /**
         * @brief Constructs a CollisionComponent with a layer ID.
         *
         * @param layerId The collision layer this entity belongs to.
         * @param isCollisionReporter Whether this entity can emit collision events.
         */
        explicit CollisionComponent(uint32_t layerId, bool isCollisionReporter = true,
            uint32_t solidCollisionMask = 0, uint32_t triggerCollisionMask = 0
        ) :
            layerId_(layerId),
            isCollisionReporter_(isCollisionReporter),
            solidCollisionMask_(solidCollisionMask),
            triggerCollisionMask_(triggerCollisionMask){}

        /**
         * @brief Copy constructor.
         *
         * @details Creates a deep copy of the collision component, preserving
         * layer ID, masks, and all behavior configurations.
         *
         * @param other The component to copy from.
         */
        explicit CollisionComponent(const CollisionComponent& other) :
            layerId_(other.layerId_),
            isCollisionReporter_(other.isCollisionReporter_),
            solidCollisionMask_(other.solidCollisionMask_),
            triggerCollisionMask_(other.triggerCollisionMask_),
            solidCollisionBehavior_(other.solidCollisionBehavior_),
            triggerCollisionBehavior_(other.triggerCollisionBehavior_)
        {}

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

        /**
         * @brief Adds a solid collision behavior for a specific layer.
         *
         * @details Associates a collision behavior with a target layer for solid collisions.
         * If a behavior was previously set for the same layer, it is replaced.
         *
         * @param collisionBehavior The behavior to apply on collision.
         * @param otherLayerId The layer ID to associate the behavior with.
         *
         * @return Reference to this component for method chaining.
         */
        CollisionComponent& addSolidCollisionBehavior(
                const helios::engine::modules::physics::collision::types::CollisionBehavior collisionBehavior,
                const uint32_t otherLayerId
        ) noexcept {

            // remove previous entries for the layerid
            for (auto& behavior : solidCollisionBehavior_) {
                behavior &= ~otherLayerId;
            }

            const auto idx = std::bit_width(std::to_underlying(collisionBehavior)) - 1;
            solidCollisionBehavior_[idx] |= otherLayerId;

            return *this;
        }

        /**
         * @brief Returns the solid collision behavior for a specific layer.
         *
         * @details Looks up the configured behavior for collisions with the specified layer.
         * Returns `CollisionBehavior::None` if no solid collision mask is set for the layer.
         *
         * @param otherLayerId The layer ID to query.
         *
         * @return The collision behavior for the specified layer.
         */
        [[nodiscard]] helios::engine::modules::physics::collision::types::CollisionBehavior solidCollisionBehavior(uint32_t otherLayerId) const noexcept {
            using helios::engine::modules::physics::collision::types::CollisionBehavior;
            if ((solidCollisionMask_ & otherLayerId) == 0) {
                return CollisionBehavior::None;
            }

            for (int i = 0; i < solidCollisionBehavior_.size(); i++) {

                if ((solidCollisionBehavior_[i] & otherLayerId) != 0) {
                    return static_cast<CollisionBehavior>(1 << i);
                }
            }

            return CollisionBehavior::None;
        }

        /**
         * @brief Returns the trigger collision behavior for a specific layer.
         *
         * @details Looks up the configured behavior for trigger collisions with the
         * specified layer. Returns `CollisionBehavior::None` if no trigger collision
         * mask is set for the layer.
         *
         * @param otherLayerId The layer ID to query.
         *
         * @return The collision behavior for the specified layer.
         */
        [[nodiscard]] helios::engine::modules::physics::collision::types::CollisionBehavior triggerCollisionBehavior(uint32_t otherLayerId) const noexcept {
            using helios::engine::modules::physics::collision::types::CollisionBehavior;
            if ((triggerCollisionMask_ & otherLayerId) == 0) {
                return CollisionBehavior::None;
            }

            for (int i = 0; i < triggerCollisionBehavior_.size(); i++) {

                if ((triggerCollisionBehavior_[i] & otherLayerId) != 0) {
                    return static_cast<CollisionBehavior>(1 << i);
                }
            }

            return CollisionBehavior::None;
        }


        /**
         * @brief Adds a trigger collision behavior for a specific layer.
         *
         * @details Associates a collision behavior with a target layer for trigger collisions.
         * If a behavior was previously set for the same layer, it is replaced.
         *
         * @param collisionBehavior The behavior to apply on collision.
         * @param otherLayerId The layer ID to associate the behavior with.
         *
         * @return Reference to this component for method chaining.
         */
        CollisionComponent& addTriggerCollisionBehavior(
                const helios::engine::modules::physics::collision::types::CollisionBehavior collisionBehavior,
                const uint32_t otherLayerId
        ) noexcept {

            // remove previous entries for the layerid
            for (auto& behavior : triggerCollisionBehavior_) {
                behavior &= ~otherLayerId;
            }

            const auto idx = std::bit_width(std::to_underlying(collisionBehavior)) - 1;
            triggerCollisionBehavior_[idx] |= otherLayerId;

            return *this;
        }

    };


}