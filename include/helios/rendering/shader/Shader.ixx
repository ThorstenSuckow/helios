/**
 * @file Shader.ixx
 * @brief Abstraction for a compiled shader program.
 */
module;

export module helios.rendering.shader.Shader;

import helios.util.log.LogManager;
import helios.util.log.Logger;

import helios.rendering.shader.UniformValueMap;

#define HELIOS_LOG_SCOPE "helios::rendering::shader::Shader"
export namespace helios::rendering::shader {

    /**
     * @brief Abstract representation of a Shader program (e.g. composed of vertex/fragment shader).
     *
     * This class defines common operations for managing and utilizing shaders.
     * Derived classes provide rendering API specifics for loading and compiling the shaders.
     */
    class Shader {

    protected:
        /**
         * @brief Shared logger instance for all Shader objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE
        );

    public:

        virtual ~Shader() = default;

        /**
         * @brief Activates this shader for subsequent rendering operations.
         * Draw calls of the current rendering pass will use this shader
         * until it is unbound or another shader is bound.
         */
        virtual void use() const noexcept = 0;

        /**
         * @brief Applies the specified UniformValueMap to the uniforms defined by the shader.
         * This method does not change the binding state of the shader, but it passes the uniform
         * values to the underlying rendering backend. Implementations should ensure
         * that the shader is properly bound before this method is called.
         *
         * @param uniformValueMap A const ref to the `UniformValueMap` containing the values for the
         * uniforms in this shader.
         */
        virtual void applyUniformValues(const UniformValueMap& uniformValueMap) const noexcept = 0;
    };

}