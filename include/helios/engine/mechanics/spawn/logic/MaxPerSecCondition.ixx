/**
 * @file MaxPerSecCondition.ixx
 * @brief Request-based spawn condition for rate-limited spawning.
 */
module;

#include <cassert>

export module helios.engine.mechanics.spawn.logic.MaxPerSecCondition;

import helios.engine.ecs.GameWorld;
import helios.engine.ecs.Level;
import helios.engine.core.data.GameObjectPoolId;
import helios.engine.ecs.UpdateContext;

import helios.engine.mechanics.spawn.logic.SpawnCondition;


export namespace helios::engine::mechanics::spawn::logic {

    /**
     * @brief Spawn condition that limits spawning based on a maximum rate per second.
     *
     * @details MaxPerSecCondition provides rate-limited spawning by calculating a spawn
     * budget based on elapsed time and a configured maximum spawn rate. This enables
     * smooth, time-distributed spawning rather than bursts.
     *
     * The spawn budget is calculated as: `maxRequestsPerSecond * deltaTime`.
     *
     * Example usage:
     * ```cpp
     * // Allow up to 10 spawns per second
     * auto condition = std::make_unique<MaxPerSecCondition>(10);
     * spawnSystem.setCondition(std::move(condition));
     * ```
     *
     * @see SpawnCondition for the base interface
     * @see TimerSpawnCondition for interval-based spawning
     */
    class MaxPerSecCondition : public SpawnCondition {

    private:

        /**
         * @brief Maximum number of spawn requests allowed per second.
         */
        float maxRequestsPerSecond_ = 0;

        /**
         * @brief The time interval between spawns in seconds.
         */
        float interval_ = 0.0f;

        /**
         * @brief The current countdown timer value in seconds.
         */
        float countdown_ = 0.0f;

    public:

        /**
         * @brief Constructs an MaxPerSecCondition with the specified spawn rate.
         *
         * @param maxRequestsPerSecond The maximum number of spawns allowed per second.
         *                             A value of 0 disables spawning entirely.
         */
        explicit MaxPerSecCondition(float maxRequestsPerSecond)
            : maxRequestsPerSecond_(maxRequestsPerSecond),
              interval_(maxRequestsPerSecond > 0 ? 1.0f / maxRequestsPerSecond : 0.0f),
              countdown_(maxRequestsPerSecond > 0 ? 1.0f / maxRequestsPerSecond : 0.0f) {}

        /**
         * @brief Calculates the spawn budget for the current frame.
         *
         * @param gameWorld The current game world state.
         * @param gameObjectPool The pool to spawn from.
         * @param updateContext The current frame's update context.
         *
         * @return The number of spawns allowed this frame (fractional values accumulate).
         */
        [[nodiscard]] float spawnBudget(
            const helios::engine::ecs::GameWorld& gameWorld,
            const helios::engine::core::data::GameObjectPool& gameObjectPool,
            const helios::engine::ecs::UpdateContext& updateContext
        ) noexcept override {

            if (maxRequestsPerSecond_ == 0.0f) {
                return 0.0f;
            }

            return maxRequestsPerSecond_ * updateContext.deltaTime();
        }

    };

}