/**
 * @file IsResourceHandle.ixx
 * @brief Concept that restricts handle types to supported rendering resource handles.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsResourceHandle;

import helios.rendering.shader.types.ShaderHandle;
import helios.rendering.material.types.MaterialHandle;
import helios.rendering.mesh.types.MeshHandle;

using namespace helios::rendering::shader::types;
using namespace helios::rendering::material::types;
using namespace helios::rendering::mesh::types;
export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains template parameters to known render-resource handle types.
     *
     * Accepted handles are `ShaderHandle`, `MaterialHandle`, and `MeshHandle`.
     *
     * @tparam THandle Candidate handle type.
     */
    template<typename THandle>
    concept IsResourceHandle =
        std::is_same_v<THandle, ShaderHandle> ||
        std::is_same_v<THandle, MaterialHandle> ||
        std::is_same_v<THandle, MeshHandle>;
}