/**
 * @file OpenGLShaderComponent.ixx
 * @brief Component storing native OpenGL program id for a shader entity.
 */
module;

export module helios.platform.opengl.components.OpenGLShaderComponent;

import helios.rendering.shader.concepts.IsShaderHandle;

using namespace helios::rendering::shader::concepts;
export namespace helios::platform::opengl::components {

    /**
     * @brief Binds a shader entity to its linked OpenGL program.
     *
     * @tparam THandle Shader handle type.
     */
    template<typename THandle>
    requires IsShaderHandle<THandle>
    struct OpenGLShaderComponent  {

        /** @brief Linked OpenGL program id. */
        unsigned int programId = 0;

    };
}