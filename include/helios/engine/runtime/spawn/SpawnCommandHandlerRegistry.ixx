/**
 * @file SpawnCommandHandlerRegistry.ixx
 * @brief Registry mapping spawn profile IDs to their command handlers.
 */
module;

#include <cassert>
#include <unordered_map>
#include <vector>

export module helios.engine.runtime.spawn.SpawnCommandHandlerRegistry;

import helios.engine.runtime.spawn.SpawnCommandHandler;
import helios.engine.core.data.SpawnProfileId;


export namespace helios::engine::runtime::spawn  {


    /**
     * @brief Registry that maps SpawnProfileIds to their SpawnCommandHandlers.
     *
     * @details SpawnCommandHandlerRegistry provides lookup from spawn profile IDs
     * to the handlers responsible for processing spawn and despawn commands for
     * those profiles. This enables the spawn/despawn system to route commands to
     * the correct manager without direct coupling.
     *
     * The registry is owned by the GameWorld and populated during manager
     * initialization.
     *
     * @note Uses amortized O(1) lookup via std::unordered_map.
     *
     * @see SpawnCommandHandler
     * @see SpawnProfileId
     * @see GameWorld::registerSpawnCommandHandler
     */
    class SpawnCommandHandlerRegistry {

    protected:

        /**
         * @brief Map from spawn profile IDs to their handlers.
         *
         * @details Stores non-owning pointers to handlers. Handlers must outlive
         * this registry.
         */
        std::unordered_map<
            helios::engine::core::data::SpawnProfileId,
            helios::engine::runtime::spawn::SpawnCommandHandler*> registry_{};


    public:

        SpawnCommandHandlerRegistry() = default;

        /**
         * @brief Registers a handler for a spawn profile ID.
         *
         * @param spawnProfileId The spawn profile ID to register.
         * @param poolManager The handler to associate with the pool.
         *
         * @return True if registration succeeded, false if the ID was already registered.
         */
        [[nodiscard]] bool add(
            const helios::engine::core::data::SpawnProfileId spawnProfileId,
            helios::engine::runtime::spawn::SpawnCommandHandler& poolManager
        ) {
            if (registry_.contains(spawnProfileId)) {
                return false;
            }

            registry_[spawnProfileId] = &poolManager;

            return true;
        }

        /**
         * @brief Retrieves the handler for a spawn profile ID.
         *
         * @param spawnProfileId The spawn profile ID to look up.
         *
         * @return Pointer to the handler if found, nullptr otherwise.
         */
        [[nodiscard]] helios::engine::runtime::spawn::SpawnCommandHandler* get(
            helios::engine::core::data::SpawnProfileId spawnProfileId) {
            auto it = registry_.find(spawnProfileId);

            if (it == registry_.end()) {
                return nullptr;
            }

            return it->second;
        }
    };


}