/**
 * @file SceneToViewportMap.ixx
 * @brief Mapping between scenes and their associated viewports.
 */
module;

#include <unordered_map>
#include <cassert>
#include <vector>
#include <span>
#include <optional>

export module helios.engine.modules.scene.types.SceneToViewportMap;

import helios.engine.core.data;
import helios.scene.Scene;
import helios.rendering.Viewport;

export namespace helios::engine::modules::scene::types {

    using namespace helios::engine::core::data;
    using namespace helios::scene;
    using namespace helios::rendering;

    /**
     * @brief Maps scenes to their associated viewports for rendering.
     *
     * @details SceneToViewportMap maintains the relationship between Scene
     * objects and Viewport objects. A scene can be rendered through multiple
     * viewports (e.g., split-screen), and a viewport renders exactly one scene.
     *
     * The map uses lazy caching: the internal lookup structures are rebuilt
     * only when `updateCache()` is called after modifications.
     *
     * ## Usage
     *
     * ```cpp
     * SceneToViewportMap map;
     * map.add(&gameScene, &mainViewport);
     * map.add(&uiScene, &hudViewport);
     *
     * // Query scene for viewport
     * auto* scene = map.scene(viewportId);
     *
     * // Query viewport for scene
     * auto* viewport = map.viewport(sceneId, viewportId);
     * ```
     *
     * @see SceneRenderingSystem
     * @see Scene
     * @see Viewport
     */
    class SceneToViewportMap {

        /**
         * @brief All registered scenes.
         */
        mutable std::vector<Scene*> scenes_;

        /**
         * @brief All registered viewports.
         */
        mutable std::vector<Viewport*> viewports_;

        /**
         * @brief Maps scene IDs to their associated viewport IDs.
         */
        mutable std::unordered_map<SceneId, std::vector<ViewportId>> sceneIdsToViewportIds_;

        /**
         * @brief Cached mapping from scene IDs to viewport pointers.
         */
        mutable std::unordered_map<SceneId, std::vector<Viewport*>> viewportMap_;

        /**
         * @brief Flag indicating the cache needs rebuilding.
         */
        mutable bool needsUpdate_ = false;

    public:

        /**
         * @brief Default constructor.
         */
        SceneToViewportMap() = default;

        /**
         * @brief Rebuilds the internal cache if modifications were made.
         *
         * @details Called lazily by query methods. Rebuilds the viewportMap_
         * from the scene-to-viewport-ID mappings.
         */
        void updateCache() const {

            if (!needsUpdate_) {
                return;
            }

            viewportMap_.clear();
            for (const auto& [sceneId, viewportIds] : sceneIdsToViewportIds_) {
                for (auto viewportId : viewportIds) {
                    for (auto viewport : viewports_) {
                        if (viewport->viewportId() == viewportId) {
                            viewportMap_[sceneId].push_back(viewport);
                        }
                    }
                }
            }


            needsUpdate_ = false;
        }

        /**
         * @brief Registers a scene-viewport association.
         *
         * @details Adds the viewport to the scene's viewport list. A scene can
         * have multiple viewports, but each viewport can only be added once.
         *
         * @param scene Pointer to the scene. Must not be null.
         * @param viewport Pointer to the viewport. Must not be null or already registered.
         */
        void add(Scene* scene, Viewport* viewport) {

            const bool viewportExists = std::ranges::find(viewports_, viewport) != viewports_.end();
            assert(!viewportExists && "Viewport already exists");
            if (viewportExists) {
                return;
            }

            const bool sceneExists = hasScene(scene->sceneId());

            sceneIdsToViewportIds_[scene->sceneId()].push_back(viewport->viewportId());
            viewports_.push_back(viewport);

            if (!sceneExists) {
                scenes_.push_back(scene);
            }

            needsUpdate_ = true;
        }

        /**
         * @brief Returns the scene associated with a viewport ID.
         *
         * @param viewportId The viewport identifier to look up.
         *
         * @return Pointer to the scene, or nullptr if not found.
         */
        Scene* scene(ViewportId viewportId) const noexcept {
            updateCache();


            for (auto [sceneId, viewportIds] : sceneIdsToViewportIds_) {
                if (std::ranges::find(viewportIds, viewportId) != viewportIds.end()) {
                    for (auto scene : scenes_) {
                        if (scene->sceneId() == sceneId) {
                            return scene;
                        }
                    }
                }
            }

            return nullptr;
        }

        /**
         * @brief Returns all viewports associated with a scene.
         *
         * @param sceneId The scene identifier to look up.
         *
         * @return Span of viewport pointers, or empty span if not found.
         */
        std::span<Viewport*> viewports(SceneId sceneId) const noexcept {
            updateCache();

            if (!hasScene(sceneId)) {
                return {};
            }

            return viewportMap_[sceneId];
        }

        /**
         * @brief Returns all registered scenes.
         *
         * @return Span of scene pointers.
         */
        std::span<Scene*> scenes() const noexcept {
            return scenes_;
        }

        /**
         * @brief Checks if a scene is registered.
         *
         * @param sceneId The scene identifier to check.
         *
         * @return True if the scene is registered, false otherwise.
         */
        bool hasScene(const SceneId sceneId) const noexcept {
            return sceneIdsToViewportIds_.contains(sceneId);
        }

        /**
         * @brief Returns a specific viewport for a scene.
         *
         * @param sceneId The scene identifier.
         * @param viewportId The viewport identifier.
         *
         * @return Pointer to the viewport, or nullptr if not found.
         */
        Viewport* viewport(SceneId sceneId, ViewportId viewportId) const noexcept {
            updateCache();

            if (!hasScene(sceneId)) {
                return nullptr;
            }

            for (auto viewport : viewportMap_[sceneId]) {
                if (viewport->viewportId() == viewportId) {
                    return viewport;
                }
            }

            return nullptr;
        }
    };



}