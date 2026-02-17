/**
 * @file SceneSyncSystem.ixx
 * @brief System for synchronizing gameplay transforms with the scene graph.
 */
module;


#include <cassert>
#include <vector>

export module helios.engine.modules.scene.systems.SceneSyncSystem;

import helios.scene.Scene;

import helios.engine.modules.scene.types.SceneToViewportMap;

import helios.engine.runtime.world.GameWorld;
import helios.engine.ecs.System;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.engine.modules.scene.components.SceneNodeComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;

export namespace helios::engine::modules::scene::systems {

    /**
     * @brief System that synchronizes TransformComponents with SceneNodes.
     *
     * @details This system bridges the gameplay simulation and the rendering scene graph.
     * It propagates local transformations from dirty ComposeTransformComponents to their
     * corresponding SceneNodes, triggers a scene graph update, and then reads back
     * the computed world transforms into the ComposeTransformComponents.
     *
     * The system runs in two passes:
     * 1. **Write pass**: Dirty local transforms are pushed to SceneNodes
     * 2. **Scene update**: All scenes update their node hierarchies
     * 3. **Read pass**: Computed world transforms are read back to components
     *
     * @see ComposeTransformComponent
     * @see SceneNodeComponent
     * @see SceneToViewportMap
     */
    class SceneSyncSystem : public helios::engine::ecs::System {

        /**
         * @brief Reference to the scene-to-viewport mapping for scene iteration.
         */
        helios::engine::modules::scene::types::SceneToViewportMap& sceneToViewportMap_;

    public:

        /**
         * @brief Constructs the system with the scene-to-viewport map.
         *
         * @param sceneToViewportMap Reference to the scene mapping for accessing all scenes.
         */
        explicit SceneSyncSystem(helios::engine::modules::scene::types::SceneToViewportMap& sceneToViewportMap)
        : sceneToViewportMap_(sceneToViewportMap) {}

        /**
         * @brief Synchronizes gameplay transforms with the scene graph.
         *
         * @details Performs a two-pass synchronization:
         * 1. Writes dirty local transforms to SceneNodes
         * 2. Updates all scene hierarchies
         * 3. Reads computed world transforms back to components
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, tc, nc, active] : gameWorld_->view<
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent,
                helios::engine::modules::scene::components::SceneNodeComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

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

            //Propagate changes and update the nodes
            for (auto* scene : sceneToViewportMap_.scenes()) {
                scene->updateNodes();
            }

            // Second pass: read back world transforms from SceneNode to ComposeTransformComponent
            for (auto [entity, tc, nc, active] : gameWorld_->view<
               helios::engine::modules::spatial::transform::components::ComposeTransformComponent,
               helios::engine::modules::scene::components::SceneNodeComponent,
               helios::engine::mechanics::lifecycle::components::Active
           >().whereEnabled()) {

                auto* sceneNode = nc->sceneNode();
                if (!sceneNode || !sceneNode->isActive()) {
                   continue;
                }

                tc->setWorldTransform(sceneNode->worldTransform());
            }

        }

    };

}