/**
 * @file IsFrustumCullerLike.ixx
 * @brief Concept that validates frustum-culling strategy types.
 */
module;

#include<concepts>

export module helios.engine.common.concepts.IsFrustumCullerLike;

import helios.ecs.types.EntityHandle;
import helios.scene.types.PerspectiveCameraContext;

using namespace helios::ecs::types;
using namespace helios::scene::types;
export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains types that expose the culling API used by scene extraction systems.
     *
     * A valid type provides `cull(const PerspectiveCameraContext&, const EntityHandle)`
     * returning `void`.
     *
     * @tparam T Candidate culling strategy type.
     * @tparam TStrongId type for the EntityHandle's StrongId.
     */
    template <typename T, typename TStrongId >
    concept IsFrustumCullerLike = requires(T& t, const EntityHandle<TStrongId> entityHandle, const PerspectiveCameraContext& cameraContext)
    {
        {t.cull(cameraContext, entityHandle)}-> std::same_as<void>;

    };

}