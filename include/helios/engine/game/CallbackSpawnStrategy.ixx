/**
 * @file CallbackSpawnStrategy.ixx
 * @brief Callback-based spawn strategy for flexible GameObject initialization.
 */
module;


#include <functional>

export module helios.engine.game.CallbackSpawnStrategy;

import helios.engine.game.SpawnStrategy;
import helios.engine.game.UpdateContext;

import helios.engine.game.GameWorld;
import helios.engine.game.GameObject;
import helios.engine.game.Level;


export namespace helios::engine::game {

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
            const helios::engine::game::GameWorld*,
            const helios::engine::game::Level*,
            helios::engine::game::GameObject*,
            const helios::engine::game::UpdateContext&)>;

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
            const helios::engine::game::GameWorld* gameWorld,
            const helios::engine::game::Level* level,
            helios::engine::game::GameObject* gameObject,
            const helios::engine::game::UpdateContext& updateContext
        ) noexcept override {
            return spawnFunction_(gameWorld, level, gameObject, updateContext);
        }

    };


}