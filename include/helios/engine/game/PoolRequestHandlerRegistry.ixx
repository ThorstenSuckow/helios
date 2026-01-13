/**
 * @file PoolRequestHandlerRegistry.ixx
 * @brief Registry mapping pool IDs to their request handlers.
 */
module;

#include <cassert>
#include <unordered_map>
#include <vector>

export module helios.engine.game.PoolRequestHandlerRegistry;

import helios.engine.core.data.GameObjectPoolId;
import helios.engine.game.PoolRequestHandler;


export namespace helios::engine::game {

    /**
     * @brief Registry that maps GameObjectPoolIds to their PoolRequestHandlers.
     *
     * @details PoolManagerRegistry provides lookup from pool IDs to the handlers
     * responsible for processing spawn and despawn requests for those pools.
     * This enables the spawn/despawn system to route requests to the correct
     * manager without direct coupling.
     *
     * The registry is owned by the GameWorld and populated during manager
     * initialization.
     *
     * ## Usage
     *
     * ```cpp
     * // During manager init
     * registry.add(bulletPoolId, bulletManager);
     *
     * // During despawn processing
     * auto* handler = registry.get(poolId);
     * if (handler) {
     *     handler->submit(despawnRequest);
     * }
     * ```
     *
     * @note Uses amortized O(1) lookup via std::unordered_map.
     *
     * @see PoolRequestHandler
     * @see GameObjectPoolId
     * @see GameWorld::registerPoolRequestHandler
     */
    class PoolManagerRegistry {

    protected:

        /**
         * @brief Map from pool IDs to their handlers.
         *
         * @details Stores non-owning pointers to handlers. Handlers must outlive
         * this registry.
         */
        std::unordered_map<helios::engine::core::data::GameObjectPoolId, helios::engine::game::PoolRequestHandler*> registry_{};


    public:

        PoolManagerRegistry() = default;

        /**
         * @brief Registers a handler for a pool ID.
         *
         * @param gameObjectPoolId The pool ID to register.
         * @param poolManager The handler to associate with the pool.
         *
         * @return True if registration succeeded, false if the ID was already registered.
         */
        [[nodiscard]] bool add(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            helios::engine::game::PoolRequestHandler& poolManager
        ) {
            if (registry_.contains(gameObjectPoolId)) {
                return false;
            }

            registry_[gameObjectPoolId] = &poolManager;

            return true;
        }

        /**
         * @brief Retrieves the handler for a pool ID.
         *
         * @param gameObjectPoolId The pool ID to look up.
         *
         * @return Pointer to the handler if found, nullptr otherwise.
         */
        [[nodiscard]] helios::engine::game::PoolRequestHandler* get(helios::engine::core::data::GameObjectPoolId gameObjectPoolId) {
            auto it = registry_.find(gameObjectPoolId);

            if (it == registry_.end()) {
                return nullptr;
            }

            return it->second;
        }
    };


}