/**
 * @file SpawnStrategy.ixx
 * @brief Abstract interface for GameObject spawning strategies.
 */
module;


export module helios.engine.game.SpawnStrategy;


import helios.engine.game.UpdateContext;

import helios.engine.game.GameWorld;
import helios.engine.game.GameObject;
import helios.engine.game.Level;


export namespace helios::engine::game {

    /**
     * @brief Abstract base class defining how GameObjects are spawned.
     *
     * @details
     * SpawnStrategy implements the Strategy pattern for GameObject spawning.
     * Concrete implementations define the placement, initialization, and
     * configuration of GameObjects when they are activated by a spawn system.
     *
     * Example usage:
     * ```cpp
     * class RandomSpawnStrategy : public SpawnStrategy {
     * public:
     *     bool spawn(const GameWorld*, const Level* level,
     *                GameObject* obj, const UpdateContext&) noexcept override {
     *         auto  rng = helios::util::Random(12345);
     *         float x   = rng.randomFloat(level->bounds().min[0], level->bounds().max[0]);
     *         float y   = rng.randomFloat(level->bounds().min[1], level->bounds().max[1]);
     *         obj->get<TranslationStateComponent>()->setTranslation({x, y, 0});
     *         return true;
     *     }
     * };
     * ```
     *
     * @see CallbackSpawnStrategy
     * @see GameObjectSpawnSystem
     */
    class SpawnStrategy {

    public:

        /**
         * @brief Default constructor.
         */
        SpawnStrategy() = default;

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~SpawnStrategy() = default;

        /**
         * @brief Spawns and configures a GameObject.
         *
         * @details
         * Called by the spawn system when a GameObject is acquired from a pool
         * and needs to be initialized for gameplay. The implementation should
         * configure the object's position, direction, and any other spawn-specific
         * state.
         *
         * @param gameWorld Pointer to the current GameWorld. May be nullptr.
         * @param level Pointer to the current Level containing bounds and spawn info.
         * @param gameObject Pointer to the GameObject being spawned.
         * @param updateContext The current frame's update context.
         *
         * @return True if spawning was successful and the object should remain active.
         *         False if spawning failed and the object should be returned to the pool.
         */
        virtual bool spawn(
            const helios::engine::game::GameWorld* gameWorld,
            const helios::engine::game::Level* level,
            helios::engine::game::GameObject* gameObject,
            const helios::engine::game::UpdateContext& updateContext
        ) noexcept = 0;

    };


}