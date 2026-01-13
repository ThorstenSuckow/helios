/**
 * @file SceneSyncSystem.ixx
 * @brief System for synchronizing gameplay transforms with the scene graph.
 */
module;

#include <helios/engine/ecs/query/GameObjectView.h>
#include <cassert>

export module helios.engine.game.scene.systems.SceneSyncSystem;

import helios.scene.Scene;

import helios.engine.ecs.GameWorld;
import helios.engine.ecs.System;
import helios.engine.ecs.UpdateContext;

import helios.engine.game.scene.components.SceneNodeComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;

export namespace helios::engine::game::systems::scene {

    /**
     * @brief System that synchronizes TransformComponents with SceneNodes.
     *
     * @details This system bridges the gameplay simulation and the rendering scene graph.
     * It propagates local transformations from dirty TransformComponents to their
     * corresponding SceneNodes, triggers a scene graph update, and then reads back
     * the computed world transforms into the TransformComponents.
     */
    class SceneSyncSystem : public helios::engine::ecs::System {

        /**
         * @brief Pointer to the Scene being synchronized.
         */
        helios::scene::Scene* scene_;

    public:

        /**
         * @brief Constructs a SceneSyncSystem.
         *
         * @param scene Pointer to the Scene to synchronize with. Must not be nullptr.
         */
        explicit SceneSyncSystem(helios::scene::Scene* scene) : scene_(scene) {
            assert(scene_ != nullptr && "SceneSyncSystem requires Scene");
        }

        /**
         * @brief Updates the system, synchronizing transforms.
         *
         * @param updateContext Context containing frame timing and game state.
         */
        void update(helios::engine::ecs::UpdateContext& updateContext) noexcept override {

            auto view = gameWorld_->find<
                helios::engine::game::spatial::transform::components::TransformComponent,
                helios::engine::game::scene::components::SceneNodeComponent
            >();

            // First pass: push local transforms from TransformComponent to SceneNode
            for (auto [entity, tc, nc] : view.each()) {

                if (!tc->isDirty()) {
                    continue;
                }

                auto* sceneNode = nc->sceneNode();
                if (!sceneNode || !sceneNode->isActive()) {
                    continue;
                }

                sceneNode->setScale(tc->localScaling());
                sceneNode->setRotation(tc->localRotation());
                sceneNode->setTranslation(tc->localTranslation());
            }

            // Propagate changes and update the nodes
            scene_->updateNodes();

            // Second pass: read back world transforms from SceneNode to TransformComponent
            for (auto [entity, tc, nc] : view.each()) {

                if (!tc->isDirty()) {
                    continue;
                }

                auto* sceneNode = nc->sceneNode();
                if (!sceneNode || !sceneNode->isActive()) {
                    continue;
                }

                tc->setWorldTransform(sceneNode->worldTransform());
            }

        }

    };

}