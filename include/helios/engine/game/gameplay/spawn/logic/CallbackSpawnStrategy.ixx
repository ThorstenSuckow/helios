/**
 * @file CallbackSpawnStrategy.ixx
 * @brief Callback-based spawn strategy for flexible GameObject initialization.
 */
module;


#include <functional>

export module helios.engine.game.gameplay.spawn.logic.CallbackSpawnStrategy;

import helios.engine.game.gameplay.spawn.logic.SpawnStrategy;
import helios.engine.ecs.UpdateContext;

import helios.engine.ecs.GameWorld;
import helios.engine.ecs.GameObject;
import helios.engine.ecs.Level;


export namespace helios::engine::game::gameplay::spawn::logic {

    /**
     * @brief A SpawnStrategy that delegates spawning logic to a callback function.
     *
     * @details
     * CallbackSpawnStrategy provides a lightweight way to define custom spawn
     * behavior without creating a dedicated subclass. The spawn logic is
     * encapsulated in a `std::function` passed at construction time.
     *
     * Useful for simple or one-off spawn configurations, rapid prototyping,
     * or when the spawn logic is defined dynamically.
     *
     * Example usage:
     * ```cpp
     * auto strategy = std::make_unique<CallbackSpawnStrategy>(
     *     [](const GameWorld*, const Level* level, GameObject* obj, const UpdateContext&) {
     *         auto rng = helios::util::Random(1234);
     *         float x = rng.randomFloat(-10.0f, 10.0f);
     *         float y = rng.randomFloat(-10.0f, 10.0f);
     *         obj->get<TransformComponent>()->setTranslation({x, y, 0});
     *         return true;
     *     }
     * );
     * ```
     *
     * @see SpawnStrategy
     * @see GameObjectSpawnSystem
     */
    class CallbackSpawnStrategy : public SpawnStrategy {

        /**
         * @brief Function signature for the spawn callback.
         *
         * Must not throw
         */
        using SpawnFunction = std::function<bool(
            const helios::engine::ecs::GameWorld&,
            helios::engine::ecs::GameObject&,
            const helios::engine::ecs::UpdateContext&)>;

        /**
         * @brief The user-provided spawn callback function.
         */
        SpawnFunction spawnFunction_;

    public:

        /**
         * @brief Constructs a CallbackSpawnStrategy with the given spawn function.
         *
         * @param spawnFunction The callback function to invoke during spawning.
         *                      Must be a valid, noexcept callable.
         */
        explicit CallbackSpawnStrategy(SpawnFunction spawnFunction) noexcept :
            spawnFunction_(std::move(spawnFunction)) {}

        /**
         * @brief Spawns a GameObject by invoking the stored callback function.
         *
         * @param gameWorld Pointer to the current GameWorld. May be nullptr.
         * @param level Pointer to the current Level containing bounds and spawn info.
         * @param gameObject Pointer to the GameObject being spawned.
         * @param updateContext The current frame's update context.
         *
         * @return The result of the callback function. True if spawning succeeded,
         *         false if the object should be returned to the pool.
         */
        [[nodiscard]] bool spawn(
            const helios::engine::ecs::GameWorld& gameWorld,
            helios::engine::ecs::GameObject& gameObject,
            const helios::engine::ecs::UpdateContext& updateContext
        ) noexcept override {
            return spawnFunction_(gameWorld, gameObject, updateContext);
        }

    };


}