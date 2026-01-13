/**
 * @file PoolIdComponent.ixx
 * @brief Component that stores a GameObject's pool membership.
 */
module;

#include <memory>

export module helios.engine.game.pool.components.PoolIdComponent;

import helios.engine.game.CloneableComponent;
import helios.engine.core.data.GameObjectPoolId;

export namespace helios::engine::game::pool::components {

    /**
     * @brief Component that identifies which pool a GameObject belongs to.
     *
     * @details PoolIdComponent stores the GameObjectPoolId of the pool that
     * owns this entity. This is essential for:
     *
     * - **Despawn Routing:** When a pooled entity needs to be returned, the
     *   PoolIdComponent tells the system which pool's handler to notify.
     * - **Pool Membership:** Identifies entities that belong to a specific pool.
     *
     * The component is cloneable, so pooled object templates can be duplicated
     * while preserving their pool association.
     *
     * Example usage:
     * ```cpp
     * // During pool population
     * gameObject->add<PoolIdComponent>(bulletPoolId);
     *
     * // During despawn
     * auto* pic = gameObject->get<PoolIdComponent>();
     * if (pic) {
     *     auto handler = gameWorld.poolRequestHandler(pic->gameObjectPoolId());
     *     handler->submit(DespawnRequest{gameObject->guid(), pic->gameObjectPoolId()});
     * }
     * ```
     *
     * @see GameObjectPool
     * @see GameObjectPoolId
     * @see PoolRequestHandler
     */
    class PoolIdComponent : public helios::engine::game::CloneableComponent<PoolIdComponent> {

        /**
         * @brief The ID of the pool this GameObject belongs to.
         */
        const helios::engine::core::data::GameObjectPoolId gameObjectPoolId_;

    public:

        /**
         * @brief Constructs a PoolIdComponent with the specified pool ID.
         *
         * @param gameObjectPoolId The ID of the pool this entity belongs to.
         */
        explicit PoolIdComponent(helios::engine::core::data::GameObjectPoolId gameObjectPoolId)
        : gameObjectPoolId_(gameObjectPoolId) {}

        /**
         * @brief Copy constructor for cloning.
         *
         * @param other The component to copy from.
         */
        explicit PoolIdComponent(const PoolIdComponent& other)
                : gameObjectPoolId_(other.gameObjectPoolId_) {}

        /**
         * @brief Returns the pool ID this entity belongs to.
         *
         * @return The GameObjectPoolId.
         */
        [[nodiscard]] helios::engine::core::data::GameObjectPoolId gameObjectPoolId() const noexcept {
            return gameObjectPoolId_;
        }


    };


}