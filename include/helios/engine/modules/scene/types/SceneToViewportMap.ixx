/**
 * @file SceneToViewportMap.ixx
 * @brief Mapping between scene handles and their associated viewport handles.
 */
module;

#include <vector>
#include <span>
#include <cassert>

export module helios.engine.modules.scene.types.SceneToViewportMap;

import helios.rendering.viewport.types.ViewportHandle;
import helios.scene.types.SceneHandle;

using namespace helios::scene::types;
using namespace helios::rendering::viewport::types;

export namespace helios::engine::modules::scene::types {

    /**
     * @brief Maps scenes to their associated viewports for rendering.
     *
     * @details SceneToViewportMap maintains the relationship between Scene
     * objects and Viewport objects. A scene can be rendered through multiple
     * viewports (e.g., split-screen), and a viewport renders exactly one scene (1:N).
     *
     * @see SceneRenderingSystem
     * @see Scene
     * @see Viewport
     */
    class SceneToViewportMap {

        /**
         * @brief Scene-indexed list of associated viewport handles.
         */
        std::vector<std::vector<ViewportHandle>> sceneToViewportHandles_;

        /**
         * @brief Viewport-indexed reverse lookup to the owning scene handle.
         */
        std::vector<SceneHandle> viewportToSceneHandles_;

    public:

        explicit SceneToViewportMap() = default;

        /**
         * @brief Registers a scene-viewport association.
         *
         * @details Adds the viewport to the scene's viewport list. A scene can
         * have multiple viewports, but each viewport can only be added once.
         *
         * @param sceneHandle Handle of the scene to associate.
         * @param viewportHandle Handle of the viewport to register.
         *
         * @return true if the viewportHandle was successfully registered for the SceneHandle,
         * otehrwise false
         */
        bool add(const SceneHandle sceneHandle, const ViewportHandle viewportHandle) {

            const auto denseSceneIdx = sceneHandle.entityId;
            const auto denseViewportIdx = viewportHandle.entityId;

            // assert that the viewport was not already registered.
            if (viewportHandle.entityId >= viewportToSceneHandles_.size()
            || !viewportToSceneHandles_[denseViewportIdx].isValid()) {
                assert(false && "Viewport already exists");
                return false;
            }

            // scene idx to viewport handles
            if (sceneToViewportHandles_.size() <= denseSceneIdx) {
                sceneToViewportHandles_.resize(denseSceneIdx + 1);
            }
            sceneToViewportHandles_[denseSceneIdx].push_back(viewportHandle);

            // viewport idx to scene handles
            if (viewportToSceneHandles_.size() <= denseViewportIdx) {
                viewportToSceneHandles_.resize(denseViewportIdx + 1);
            }
            viewportToSceneHandles_[denseViewportIdx] = sceneHandle;

            return true;
        }

        /**
         * @brief Returns the scene associated with a viewport.
         *
         * @param viewportHandle Handle of the viewport to look up.
         *
         * @return The associated scene handle, or an invalid handle if not found.
         */
        [[nodiscard]] SceneHandle sceneHandle(const ViewportHandle viewportHandle) const noexcept {
            const auto denseViewportIdx = viewportHandle.entityId;

            if (viewportToSceneHandles_.size() <= denseViewportIdx) {
                return SceneHandle{};
            }

            return viewportToSceneHandles_[denseViewportIdx];
        }

        /**
         * @brief Returns all viewports associated with a scene.
         *
         * @param sceneHandle Handle of the scene to look up.
         *
         * @return A span of viewport handles, or an empty span if not found.
         */
        [[nodiscard]] std::span<const ViewportHandle> viewportHandles(const SceneHandle sceneHandle) const noexcept {

            const auto denseSceneIdx = sceneHandle.entityId;

            if (sceneToViewportHandles_.size() <= denseSceneIdx) {
                return {};
            }

            return sceneToViewportHandles_[denseSceneIdx];
        }


    };

}