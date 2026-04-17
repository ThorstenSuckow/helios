/**
 * @file IsShaderHandle.ixx
 * @brief Concept constraining shader handle types.
 */
module;

#include <concepts>

export module helios.rendering.shader.concepts.IsShaderHandle;

import helios.rendering.shader.types.ShaderHandle;

using namespace helios::rendering::shader::types;
export namespace helios::rendering::shader::concepts {

    /**
     * @brief Constrains a type to the shader handle alias.
     *
     * @tparam T Type to validate.
     */
    template<class T>
    concept IsShaderHandle = std::is_same_v<T, ShaderHandle>;

}