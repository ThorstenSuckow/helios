/**
 * @file TimerSpawnCondition.ixx
 * @brief Time-based spawn condition for interval-controlled spawning.
 */
module;

#include <cassert>

export module helios.engine.game.gameplay.spawn.logic.TimerSpawnCondition;

import helios.engine.game.GameWorld;
import helios.engine.game.Level;
import helios.engine.core.data.GameObjectPool;
import helios.engine.game.UpdateContext;

import helios.engine.game.gameplay.spawn.logic.SpawnCondition;


export namespace helios::engine::game::gameplay::spawn::logic {

    /**
     * @brief A SpawnCondition that triggers spawning at fixed time intervals.
     *
     * @details
     * TimerSpawnCondition uses a countdown timer to determine when GameObjects
     * should be spawned. Each time the countdown reaches zero, it resets and
     * increments the spawn budget. If the elapsed time exceeds multiple intervals
     * (e.g., during a frame spike), multiple objects will be budgeted for spawning.
     *
     * Example usage:
     * ```cpp
     * // Spawn one object every 2 seconds
     * auto condition = std::make_unique<TimerSpawnCondition>(2.0f);
     *
     * auto spawnSystem = std::make_unique<GameObjectSpawnSystem>(
     *     std::move(pool), level, std::move(condition), std::move(strategy)
     * );
     * ```
     *
     * @see SpawnCondition
     * @see GameObjectSpawnSystem
     */
    class TimerSpawnCondition : public SpawnCondition {

    private:

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
         * @brief Constructs a TimerSpawnCondition with the specified interval.
         *
         * Countdown gets initialized with interval to make sure the first spawn occurs
         * after interval seconds.
         *
         * @param interval The time in seconds between spawn events. Must be greater than 0.
         *
         * @pre interval > 0.0f
         */
        explicit TimerSpawnCondition(float interval)
            : interval_(interval), countdown_(interval) {
            assert(interval > 0.0f && "Interval must be > 0");
        }

        /**
         * @brief Calculates the spawn budget based on elapsed time.
         *
         * @details
         * Decrements the countdown by the frame's delta time. For each interval
         * that has elapsed, increments the spawn budget by one. This handles
         * frame spikes gracefully by spawning multiple objects if needed.
         *
         * @param gameWorld Pointer to the current GameWorld. Unused.
         * @param level Pointer to the current Level. Unused.
         * @param gameObjectPool Pointer to the pool. Unused.
         * @param updateContext The current frame's update context with delta time.
         *
         * @return The number of objects to spawn this frame based on elapsed intervals.
         */
        [[nodiscard]] size_t spawnBudget(
            const helios::engine::game::GameWorld* gameWorld,
            const helios::engine::game::Level* level,
            const helios::engine::game::GameObjectPool* gameObjectPool,
            const helios::engine::game::UpdateContext& updateContext
        ) noexcept override {

            size_t budget = 0;

            countdown_ -= updateContext.deltaTime();

            if (countdown_ <= 0) {
                const float spill = -countdown_;
                const auto intervalsMissed = static_cast<size_t>(spill/interval_);
                const auto spawnCount = intervalsMissed + 1U; // 1U accounts for the current call

                budget += spawnCount;
                countdown_ += static_cast<float>(spawnCount) * interval_;
            }

            return budget;
        }

    };

}