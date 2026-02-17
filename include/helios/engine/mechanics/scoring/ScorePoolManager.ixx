/**
 * @file ScorePoolManager.ixx
 * @brief Manager for score pools and score command handling.
 */
module;

#include <cassert>
#include <memory>
#include <stdexcept>
#include <unordered_map>

export module helios.engine.mechanics.scoring.ScorePoolManager;

import helios.engine.mechanics.scoring.ScorePool;
import helios.engine.mechanics.scoring.types.ScoreValueContext;

import helios.engine.mechanics.scoring.ScoreCommandHandler;

import helios.engine.core.data.ScorePoolId;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.pooling.GameObjectPool;

import helios.core.types;
import helios.util.Guid;

export namespace helios::engine::mechanics::scoring {


    /**
     * @brief Manages score pools and processes score commands.
     *
     * @details ScorePoolManager is responsible for creating and managing score pools,
     * and for handling score update commands. It implements both the Manager
     * interface (for lifecycle management) and ScoreCommandHandler (for
     * receiving score commands from the command system).
     *
     * Score updates are batched: commands submitted via `submit()` are queued
     * and processed during `flush()`, ensuring deterministic ordering.
     *
     * @see ScorePool
     * @see ScoreCommandHandler
     * @see Manager
     */
    class ScorePoolManager : public helios::engine::runtime::world::Manager,
                             public helios::engine::mechanics::scoring::ScoreCommandHandler {

        /**
         * @brief Collection of score pools managed by this manager.
         */
        std::vector<ScorePool> pools_;

        /**
         * @brief Pending score contexts to be processed on flush.
         */
        std::vector<helios::engine::mechanics::scoring::types::ScoreValueContext> scores_;


    public:


        /**
         * @brief Creates and registers a new score pool.
         *
         * @param scorePoolId Unique identifier for the new pool.
         *
         * @return Reference to the newly created ScorePool.
         */
        ScorePool& addScorePool(helios::engine::core::data::ScorePoolId scorePoolId) noexcept {

            assert(!scorePool(scorePoolId) && "Score with scorePoolId already registered");

            pools_.emplace_back(scorePoolId);

            return pools_.back();
        }


        /**
         * @brief Retrieves a score pool by ID.
         *
         * @param scorePoolId The ID of the pool to find.
         *
         * @return Pointer to the ScorePool, or nullptr if not found.
         */
        [[nodiscard]] ScorePool* scorePool(const helios::engine::core::data::ScorePoolId scorePoolId) noexcept {

            const auto it = std::ranges::find_if(pools_, [&scorePoolId](const auto& scorePool) -> bool {
                return scorePool.scorePoolId() == scorePoolId;
            });

            if (it == pools_.end()) {
                return nullptr;
            }

            return &*it;
        }


        /**
         * @brief Flushes pending score updates to their respective pools.
         *
         * Processes all pending ScoreValueContext entries and adds them to
         * the appropriate score pools based on their scorePoolId.
         *
         * @param gameWorld Reference to the game world.
         * @param update_context Reference to the update context.
         */
        void flush(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& update_context
        ) noexcept override {

            for (const auto& scoreContext : scores_) {

                for (auto& pool : pools_) {
                    if (const auto scorePoolId = pool.scorePoolId(); scorePoolId == scoreContext.scorePoolId) {
                        pool.addScore(scoreContext);
                        break;
                    }
                }
            }


            scores_.clear();
        }


        /**
         * @brief Submits a score update command for processing.
         *
         * @param updateScoreCommand The command containing score context.
         *
         * @return True if the command was accepted.
         */
        bool submit(
            const helios::engine::mechanics::scoring::commands::UpdateScoreCommand& updateScoreCommand
        ) noexcept override {
            scores_.push_back(std::move(updateScoreCommand).scoreContext());

            return true;
        };

        /**
         * @brief Initializes the manager and registers it as the score command handler.
         *
         * @param gameWorld Reference to the game world.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) override {
            gameWorld.registerScoreCommandHandler(*this);
        }


        /**
         * @brief Resets all managed score pools to zero.
         *
         * @details Iterates through all registered pools and calls their reset()
         * method, clearing all scores and resetting totals.
         */
        virtual void reset() {
            for (auto& pool : pools_) {
                pool.reset();
            }
        }
    };

}
