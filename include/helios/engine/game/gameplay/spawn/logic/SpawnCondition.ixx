/**
 * @file SpawnCondition.ixx
 * @brief Abstract interface for determining spawn timing and quantity.
 */
module;


export module helios.engine.game.gameplay.spawn.logic.SpawnCondition;

import helios.engine.game.GameWorld;
import helios.engine.game.Level;
import helios.engine.core.data.GameObjectPool;
import helios.engine.game.UpdateContext;


export namespace helios::engine::game::gameplay::spawn::logic {

    /**
     * @brief Abstract base class defining when and how many GameObjects should spawn.
     *
     * @details
     * SpawnCondition implements the Strategy pattern for spawn timing control.
     * Concrete implementations determine how many objects should be spawned each
     * frame based on time, game state, pool availability, or other conditions.
     *
     * The spawn budget returned by `spawnBudget()` represents the number of
     * GameObjects that the spawn system should attempt to activate this frame.
     *
     * @see TimerSpawnCondition
     * @see GameObjectSpawnSystem
     */
    class SpawnCondition {

    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~SpawnCondition() = default;

        /**
         * @brief Calculates how many GameObjects should be spawned this frame.
         *
         * @details
         * Called each frame by the spawn system to determine how many objects
         * to activate from the pool. The implementation may consider elapsed time,
         * pool capacity, game state, or any other relevant factors.
         *
         * @param gameWorld Pointer to the current GameWorld. May be nullptr.
         * @param level Pointer to the current Level containing bounds and spawn info.
         * @param gameObjectPool Pointer to the pool from which objects will be acquired.
         * @param updateContext The current frame's update context with delta time.
         *
         * @return The number of GameObjects to spawn this frame (spawn budget).
         *         Returns 0 if no spawning should occur.
         */
        virtual size_t spawnBudget(
            const helios::engine::game::GameWorld* gameWorld,
            const helios::engine::game::Level* level,
            const helios::engine::game::GameObjectPool* gameObjectPool,
            const helios::engine::game::UpdateContext& updateContext
        ) noexcept = 0;

    };

}