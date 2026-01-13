/**
 * @file GameObjectSpawnSystem.ixx
 * @brief System for spawning GameObjects from a pool into the level.
 */
module;

#include <cassert>
#include <memory>
#include <vector>

export module helios.engine.game.gameplay.spawn.systems.GameObjectSpawnSystem;

import helios.engine.game.System;


import helios.engine.core.messaging.command.CommandBuffer;
import helios.engine.core.data.GameObjectPoolId;
import helios.engine.game.GameObject;
import helios.engine.game.UpdateContext;



import helios.engine.game.Level;

import helios.engine.game.physics.motion.components.Move2DComponent;

import helios.engine.game.scene.components.SceneNodeComponent;
import helios.engine.game.physics.motion.components.DirectionComponent;
import helios.engine.game.rendering.components.RenderableComponent;

import helios.scene.SceneNode;

import helios.engine.game.GameWorld;

import helios.engine.game.gameplay.spawn.commands.SpawnCommand;
import helios.engine.game.gameplay.spawn.logic.SpawnCondition;
import helios.engine.game.gameplay.spawn.logic.SpawnStrategy;


export namespace helios::engine::game::gameplay::spawn::systems {

    /**
     * @brief System that evaluates spawn conditions and emits SpawnCommands.
     *
     * @details GameObjectSpawnSystem integrates spawning into the game loop by
     * evaluating a SpawnCondition each frame to determine how many objects should
     * be spawned. When the spawn budget is greater than zero, it emits a SpawnCommand
     * to the CommandBuffer for deferred execution.
     *
     * The actual spawning is handled by the SpawnCommandDispatcher and SpawnManager
     * during the command flush phase.
     *
     * Example usage:
     * ```cpp
     * auto condition = std::make_unique<TimerSpawnCondition>(2.0f);
     *
     * gameLoop.phase(PhaseType::Main)
     *     .addPass()
     *     .addSystem<GameObjectSpawnSystem>(poolId, std::move(condition));
     * ```
     *
     * @see SpawnCondition
     * @see SpawnCommand
     * @see SpawnManager
     */
    class GameObjectSpawnSystem : public System {

        /**
         * @brief The ID of the pool to spawn from.
         */
        helios::engine::core::data::GameObjectPoolId gameObjectPoolId_;

        /**
         * @brief The condition that determines when and how many objects to spawn.
         */
        std::unique_ptr<helios::engine::game::gameplay::spawn::logic::SpawnCondition> spawnCondition_;


    public:

        /**
         * @brief Constructs a GameObjectSpawnSystem.
         *
         * @param gameObjectPoolId The ID of the pool to spawn from.
         * @param spawnCondition The condition that controls spawn timing. Must not be nullptr.
         *
         * @pre spawnCondition != nullptr
         */
        explicit GameObjectSpawnSystem(
            helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            std::unique_ptr<helios::engine::game::gameplay::spawn::logic::SpawnCondition> spawnCondition
        ) :
            gameObjectPoolId_(gameObjectPoolId),
            spawnCondition_(std::move(spawnCondition))
        {
            assert(spawnCondition_ != nullptr && "Unexpected nullptr for SpawnCondition");
        }

        /**
         * @brief Evaluates the spawn condition and emits SpawnCommands.
         *
         * @details Each frame, queries the spawn condition for the spawn budget.
         * If the budget is greater than zero, emits a SpawnCommand to the
         * CommandBuffer for deferred processing.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            const auto* gameObjectPool = updateContext.gameWorld().pool(gameObjectPoolId_);
            size_t spawnCount = spawnCondition_->spawnBudget(*gameWorld_, *gameObjectPool, updateContext);

            if (spawnCount > 0) {
                updateContext.commandBuffer().add<helios::engine::game::gameplay::spawn::commands::SpawnCommand>(
                    gameObjectPoolId_,
                    spawnCount
                );
            }
        }
        
    };

}
