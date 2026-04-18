/**
 * @file SceneRenderingSystem.ixx
 * @brief System for rendering scenes through their associated viewports.
 */
module;


#include <cassert>
#include <vector>

export module helios.scene.systems.SceneRenderingSystem;

import helios.scene.Scene;

import helios.rendering.RenderingDevice;

import helios.runtime.world.Session;

import helios.runtime.world.GameWorld;

import helios.runtime.world.UpdateContext;

import helios.ecs.components.Active;

import helios.rendering.RenderPassFactory;

import helios.scene.SceneToViewportMap;

import helios.scene.components.SceneNodeComponent;
import helios.spatial.transform.components.ComposeTransformComponent;
import helios.engine.common.tags.SystemRole;

using namespace helios::scene;
export namespace helios::scene::systems {

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
    template<typename THandle>
    class SceneRenderingSystem {

        /**
         * @brief Reference to the rendering device for draw call submission.
         */
        helios::rendering::RenderingDevice& renderingDevice_;

        /**
         * @brief Reference to the scene-to-viewport mapping.
         */
        SceneToViewportMap& sceneToViewportMap_;

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Constructs the system with required dependencies.
         *
         * @param renderingDevice The rendering device for submitting draw calls.
         * @param sceneToViewportMap Mapping from scenes to their viewports.
         */
        explicit SceneRenderingSystem(
            helios::rendering::RenderingDevice& renderingDevice,
            SceneToViewportMap& sceneToViewportMap
        )
        : renderingDevice_(renderingDevice), sceneToViewportMap_(sceneToViewportMap) {}

        /**
         * @brief Renders all active viewports for this frame.
         *
         * @details Iterates over the session's active viewport IDs, resolves each
         * to its scene and viewport, creates a snapshot, builds a LegacyRenderPass, and
         * submits it to the rendering device.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {


            auto& session = updateContext.session();

            auto viewportHandles = session.viewportHandles();

            for (auto viewportHandle : viewportHandles) {

                auto sceneHandle = sceneToViewportMap_.key(viewportHandle);
                if (!sceneHandle.isValid()) {
                    continue;
                }

                /*auto viewport = sceneToViewportMap_.viewport(scene->sceneHandle(), viewportId);
                if (!viewport) {
                    continue;
                }

                const auto& snapshot = scene->createSnapshot(*viewport);
                if (snapshot.has_value()) {
                    auto renderPass = helios::rendering::RenderPassFactory::getInstance()
                        .buildRenderPass(*snapshot);
                    renderingDevice_.render(renderPass);
                }*/

            }

        }

    };

}