/**
 * @file IsRenderResourceHandle.ixx
 * @brief Concept constraining render resource handle types.
 */
module;

#include <concepts>

export module helios.rendering.concepts.IsRenderResourceHandle;

import helios.rendering.shader.types.ShaderHandle;
import helios.rendering.material.types.MaterialHandle;
import helios.rendering.mesh.types.MeshHandle;

using namespace helios::rendering::shader::types;
using namespace helios::rendering::material::types;
using namespace helios::rendering::mesh::types;
export namespace helios::rendering::concepts {

    /**
     * @brief Constrains types to known render resource handles.
     *
     * @tparam T Type to validate.
     */
    template<typename T>
    concept IsRenderResourceHandle = std::is_same_v<T, ShaderHandle> ||
                                     std::is_same_v<T, MaterialHandle> ||
                                     std::is_same_v<T, MeshHandle>;

}