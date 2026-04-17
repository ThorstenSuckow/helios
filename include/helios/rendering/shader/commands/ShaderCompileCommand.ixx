/**
 * @file ShaderCompileCommand.ixx
 * @brief Command payload for compiling a single shader resource.
 */
module;

export module helios.rendering.shader.commands.ShaderCompileCommand;

import helios.rendering.shader.types.ShaderHandle;

using namespace helios::rendering::shader::types;
export namespace helios::rendering::shader::commands {

    /**
     * @brief Requests compilation of one shader handle.
     *
     * @tparam THandle Shader handle type.
     */
    template<typename THandle>
    struct ShaderCompileCommand {
        THandle shaderHandle;
    };

}