/**
 * @file SceneRenderingSystem.ixx
 * @brief System for rendering scenes through their associated viewports.
 */
module;


#include <cassert>
#include <vector>

export module helios.engine.modules.scene.systems.SceneRenderingSystem;

import helios.scene.Scene;

import helios.rendering.RenderingDevice;

import helios.engine.runtime.world.Session;

import helios.engine.runtime.world.GameWorld;
import helios.engine.ecs.System;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.rendering.RenderPassFactory;

import helios.engine.modules.scene.types.SceneToViewportMap;

import helios.engine.modules.scene.components.SceneNodeComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;

export namespace helios::engine::modules::scene::systems {

    using namespace helios::engine::modules::scene::types;

    /**
     * @brief Renders scenes through their configured viewports.
     *
     * @details This system iterates over all active viewport IDs from the session,
     * resolves the associated scene and viewport via SceneToViewportMap, creates
     * a scene snapshot, and submits it to the rendering device.
     *
     * The system runs in the Post phase after all gameplay updates are complete,
     * ensuring the scene graph reflects the final state for this frame.
     *
     * @see SceneToViewportMap
     * @see RenderingDevice
     * @see RenderPassFactory
     */
    class SceneRenderingSystem : public helios::engine::ecs::System {

        /**
         * @brief Reference to the rendering device for draw call submission.
         */
        helios::rendering::RenderingDevice& renderingDevice_;

        /**
         * @brief Reference to the scene-to-viewport mapping.
         */
        helios::engine::modules::scene::types::SceneToViewportMap& sceneToViewportMap_;

    public:

        /**
         * @brief Constructs the system with required dependencies.
         *
         * @param renderingDevice The rendering device for submitting draw calls.
         * @param sceneToViewportMap Mapping from scenes to their viewports.
         */
        explicit SceneRenderingSystem(
            helios::rendering::RenderingDevice& renderingDevice,
            helios::engine::modules::scene::types::SceneToViewportMap& sceneToViewportMap
        )
        : renderingDevice_(renderingDevice), sceneToViewportMap_(sceneToViewportMap) {}

        /**
         * @brief Renders all active viewports for this frame.
         *
         * @details Iterates over the session's active viewport IDs, resolves each
         * to its scene and viewport, creates a snapshot, builds a RenderPass, and
         * submits it to the rendering device.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {


            auto& session = updateContext.session();

            auto viewportIds = session.viewportIds();

            for (auto viewportId : viewportIds) {

                auto scene = sceneToViewportMap_.scene(viewportId);
                if (!scene) {
                    continue;
                }

                auto viewport = sceneToViewportMap_.viewport(scene->sceneId(), viewportId);
                if (!viewport) {
                    continue;
                }

                const auto& snapshot = scene->createSnapshot(*viewport);
                if (snapshot.has_value()) {
                    auto renderPass = helios::rendering::RenderPassFactory::getInstance()
                        .buildRenderPass(*snapshot);
                    renderingDevice_.render(renderPass);
                }

            }

        }

    };

}