/**
 * @file SceneId.ixx
 * @brief Strong-id type definition for scenes.
 */
module;

export module helios.scene.types.SceneId;

import helios.core.types;

using namespace helios::core::types;
export namespace helios::scene::types {

    /**
     * @brief Tag type used to specialize `StrongId` for scenes.
     */
    struct SceneIdTag{};

    /**
     * @brief Strong id identifying scene entities.
     */
    using SceneId = StrongId<SceneIdTag>;

}
