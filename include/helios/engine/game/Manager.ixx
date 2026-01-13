/**
 * @file Manager.ixx
 * @brief Abstract base class for game world managers.
 */
module;

#include <vector>

export module helios.engine.game.Manager;

import helios.engine.ecs.UpdateContext;

export namespace helios::engine::game {

    class GameWorld;

    /**
     * @brief Abstract base class for managers that process deferred operations.
     *
     * @details Managers handle cross-cutting concerns that require deferred processing
     * separate from the System update cycle. They are registered with the GameWorld
     * and receive lifecycle callbacks for initialization and per-frame flushing.
     *
     * ## Lifecycle
     *
     * 1. **Registration:** Manager is added via `GameWorld::addManager<T>()`
     * 2. **onAdd():** Called immediately after registration
     * 3. **init():** Called during GameWorld initialization (before game loop starts)
     * 4. **flush():** Called each frame after Systems complete, to process queued requests
     *
     * ## Use Cases
     *
     * - **ProjectilePoolManager:** Processes shoot requests and spawns projectiles
     * - **SpawnManager:** Handles entity spawn/despawn lifecycle
     * - **Custom Managers:** Any deferred processing that doesn't fit the System model
     *
     * Example implementation:
     * ```cpp
     * class MyManager : public Manager {
     * public:
     *     void init(GameWorld& gw) override { ... }
     *     void flush(GameWorld& gw, UpdateContext& ctx) noexcept override {
     *         // Process queued requests
     *     }
     * };
     * ```
     *
     * @see GameWorld
     * @see PoolRequestHandler
     * @see System
     */
    class Manager {

    protected:

        /**
         * @brief Pointer to the owning GameWorld.
         *
         * @details Set during onAdd() and remains valid for the Manager's lifetime.
         */
        helios::engine::ecs::GameWorld* gameWorld_;

    public:

        virtual ~Manager() = default;

        /**
         * @brief Called when the manager is added to a GameWorld.
         *
         * @details Stores a reference to the GameWorld for later use.
         * Override to perform additional registration logic.
         *
         * @param gameWorld The GameWorld this manager was added to.
         */
        virtual void onAdd(helios::engine::ecs::GameWorld& gameWorld) {
            gameWorld_ = &gameWorld;
        }

        /**
         * @brief Initializes the manager before the game loop starts.
         *
         * @details Override to perform one-time setup such as pool retrieval,
         * handler registration, or resource allocation.
         *
         * @param gameWorld The GameWorld to initialize with.
         */
        virtual void init(
            helios::engine::ecs::GameWorld& gameWorld
        ) {
            // template method
        };

        /**
         * @brief Processes queued requests each frame.
         *
         * @details Called during the game loop's manager flush phase, after all
         * Systems have completed their updates. Override to process any queued
         * operations (spawn requests, despawn requests, etc.).
         *
         * @param gameWorld The GameWorld for entity access.
         * @param update_context The current frame's update context.
         */
        virtual void flush(
            helios::engine::ecs::GameWorld& gameWorld,
            helios::engine::ecs::UpdateContext& update_context
        ) noexcept = 0;
    };


}