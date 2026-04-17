/**
 * @file SceneToViewportMap.ixx
 * @brief Mapping between scene handles and their associated viewport handles.
 */
module;


export module helios.scene.SceneToViewportMap;

import helios.core.container.HandleMultiMap;

import helios.rendering.viewport.types.ViewportHandle;
import helios.scene.types.SceneHandle;



using namespace helios::scene::types;
using namespace helios::rendering::viewport::types;
using namespace helios::core::container;
export namespace helios::scene {

    /**
     * @brief One-to-many mapping from scene handles to viewport handles.
     *
     * `SceneToViewportMap` is used to track which viewports render which scene.
     * Reverse lookup from viewport to owning scene is also supported by the
     * underlying `HandleMultiMap` container.
     */
    using SceneToViewportMap = HandleMultiMap<SceneHandle, ViewportHandle>;

}