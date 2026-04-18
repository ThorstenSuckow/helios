/**
 * @file SceneSyncSystem.ixx
 * @brief System for synchronizing gameplay transforms with the scene graph.
 */
module;


#include <cassert>
#include <vector>

export module helios.scene.systems.SceneSyncSystem;

import helios.scene.Scene;

import helios.scene.SceneToViewportMap;

import helios.runtime.world.GameWorld;

import helios.runtime.world.UpdateContext;

import helios.ecs.components.Active;

import helios.scene.components.SceneNodeComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;

import helios.engine.common.tags.SystemRole;

export namespace helios::scene::systems {

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
    template<typename THandle>
    class SceneSyncSystem {

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Reference to the scene-to-viewport mapping for scene iteration.
         */
        helios::scene::SceneToViewportMap& sceneToViewportMap_;

        /**
         * @brief Constructs the system with the scene-to-viewport map.
         *
         * @param sceneToViewportMap Reference to the scene mapping for accessing all scenes.
         */
        explicit SceneSyncSystem(helios::scene::SceneToViewportMap& sceneToViewportMap)
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
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, tc, nc, active] : updateContext.view<
                THandle,
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent<THandle>,
                helios::scene::components::SceneNodeComponent<THandle>,
                helios::ecs::components::Active<THandle>
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
            /*for (auto* scene : sceneToViewportMap_.scenes()) {
                scene->updateNodes();
            }*/

            // Second pass: read back world transforms from SceneNode to ComposeTransformComponent
            for (auto [entity, tc, nc, active] : updateContext.view<
               THandle,
               helios::engine::modules::spatial::transform::components::ComposeTransformComponent<THandle>,
               helios::scene::components::SceneNodeComponent<THandle>,
               helios::ecs::components::Active<THandle>
           >().whereEnabled()) {

               /* auto* sceneNode = nc->sceneNode();
                if (!sceneNode || !sceneNode->isActive()) {
                   continue;
                }

                tc->setWorldTransform(sceneNode->worldTransform());*/
            }

        }

    };

}