/**
 * @file ShaderSourceComponent.ixx
 * @brief Component storing source paths for shader compilation.
 */
module;

#include <string>

export module helios.rendering.shader.components.ShaderSourceComponent;

import helios.rendering.shader.concepts.IsShaderHandle;

using namespace helios::rendering::shader::concepts;
export namespace helios::rendering::shader::components {

    /**
     * @brief Source-location component for shader entities.
     *
     * @tparam THandle Shader handle type.
     */
    template<typename THandle>
    requires IsShaderHandle<THandle>
    struct ShaderSourceComponent {

        /**
         * @brief Path to the vertex shader source.
         */
        std::string vertexShaderPath;

        /**
         * @brief Path to the fragment shader source.
         */
        std::string fragmentShaderPath;

    };

}