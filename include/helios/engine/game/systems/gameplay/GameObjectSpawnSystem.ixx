/**
 * @file GameObjectSpawnSystem.ixx
 * @brief System for spawning GameObjects from a pool into the level.
 */
module;

#include <cassert>
#include <memory>
#include <vector>

export module helios.engine.game.systems.gameplay.GameObjectSpawnSystem;

import helios.engine.game.System;
import helios.engine.game.GameObjectPool;
import helios.engine.game.GameObject;
import helios.engine.game.UpdateContext;

import helios.engine.game.Level;

import helios.engine.game.physics.components.Move2DComponent;

import helios.engine.game.components.scene.SceneNodeComponent;
import helios.engine.game.physics.components.DirectionComponent;
import helios.engine.game.components.rendering.RenderableComponent;

import helios.scene.SceneNode;

import helios.engine.game.SpawnCondition;
import helios.engine.game.SpawnStrategy;


export namespace helios::engine::game::systems::gameplay {
    
    /**
     * @brief System responsible for spawning GameObjects into the level.
     *
     * @details
     * This system manages the activation of GameObjects from a pool and their
     * placement within the level. It handles the initialization of SceneNodeComponents
     * for renderable objects during the warmup phase.
     *
     * During update, it acquires inactive GameObjects from the pool, and delegates
     * Spawn condition and logic to SpawnCondition and SpawnStrategy, respectively.
     */
    class GameObjectSpawnSystem : public System {

        /**
         * @brief The pool of GameObjects to manage.
         */
        std::unique_ptr<helios::engine::game::GameObjectPool> gameObjectPool_;

        /**
         * @brief Reference to the active level.
         */
        const helios::engine::game::Level& level_;

        /**
         * @brief Strategy used to determine how GameObjects are spawned in the level.
         */
        std::unique_ptr<helios::engine::game::SpawnStrategy> spawnStrategy_;

        /**
         * @brief Strategy for determining the conditions under which GameObjects are spawned.
         */
        std::unique_ptr<helios::engine::game::SpawnCondition> spawnCondition_;

        /**
         * @brief Prepares the object pool and scene graph.
         *
         * @details
         * Iterates through all inactive objects in the pool. If an object has a
         * RenderableComponent, a corresponding SceneNode is created and attached
         * to the level's root node. A SceneNodeComponent is then added to the
         * GameObject to link it with the scene graph.
         */
        void warmup() {

            std::vector<helios::engine::game::GameObject*> gameObjects = gameObjectPool_->inactiveGameObjects();

            auto* spawnNode = level_.rootNode();

            for (auto* go : gameObjects) {
                auto* rc = go->get<helios::engine::game::components::rendering::RenderableComponent>();

                assert(rc != nullptr && "RenderableComponent must not be nullptr");

                if (!rc) {
                    continue;
                }

                auto sceneNode = std::make_unique<helios::scene::SceneNode>(rc->renderable());
                auto sceneNode_ptr = sceneNode.get();
                std::ignore = spawnNode->addNode(std::move(sceneNode));
                go->add<helios::engine::game::components::scene::SceneNodeComponent>(sceneNode_ptr);
                go->setActive(false);
            }

        }


    public:
        /**
         * @brief Constructs a new GameObjectSpawnSystem.
         *
         * @param gameObjectPool Unique pointer to the GameObjectPool to manage.
         * @param level Reference to the Level where objects will be spawned.
         */
        explicit GameObjectSpawnSystem(
            std::unique_ptr<helios::engine::game::GameObjectPool> gameObjectPool,
            const helios::engine::game::Level& level,
            std::unique_ptr<helios::engine::game::SpawnCondition> spawnCondition,
            std::unique_ptr<helios::engine::game::SpawnStrategy> spawnStrategy
        ) :
            gameObjectPool_(std::move(gameObjectPool)),
            level_(level),
            spawnCondition_(std::move(spawnCondition)),
            spawnStrategy_(std::move(spawnStrategy))
        {
            assert(spawnStrategy_ != nullptr && "Unexpected nullptr for SpawnStrategy");
            assert(spawnCondition_ != nullptr && "Unexpected nullptr for SpawnCondition");
            assert(gameObjectPool_ != nullptr && "Unexpected nullptr for GameObjectPool");
            warmup();
        }


        /**
         * @brief Updates the system by spawning GameObjects based on SpawnCondition and SpawnStrategy.
         *
         * @details Checks whether there are remaining inactive objects that could be spawned.
         * Delegates to the SpawnCondition this instance was configured with to determine how many
         * objects should be considered for spawning, then calls the SpawnStrategy for spawning
         * the GameObjects into the GameWorld.
         *
         * @param updateContext The update context for the current frame.
         */
        void update(helios::engine::game::UpdateContext& updateContext) noexcept override {

            auto available = gameObjectPool_->inactiveCount();
            if (available == 0) {
                return;
            }

            size_t spawnCount = spawnCondition_->spawnBudget(gameWorld_, &level_, gameObjectPool_.get(), updateContext);

            spawnCount = std::min(spawnCount, available);

            for (size_t i = 0; i < spawnCount; i++) {
                helios::engine::game::GameObject* go = gameObjectPool_->acquire();

                if (go == nullptr) {
                    return;
                }

                if (!spawnStrategy_->spawn(gameWorld_, &level_, go, updateContext)) {
                    gameObjectPool_->release(go);
                }
            }

        }
        
    };

}
