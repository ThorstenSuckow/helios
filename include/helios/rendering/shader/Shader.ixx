module;

#include <memory>
#include <optional>

export module helios.rendering.shader.Shader;

import helios.util.log.LogManager;
import helios.util.log.Logger;

import helios.rendering.shader.UniformLocationMap;
import helios.rendering.shader.UniformSemantics;

#define HELIOS_LOG_SCOPE "helios::rendering::shader::Shader"
export namespace helios::rendering::shader {

    /**
     * Abstract representation of a Shader program (e.g. composed of vertex/fragment shader).
     *
     * This class defines common operations for managing and utilizing shaders.
     * Derived classes provide rendering API-specifics for loading and compiling the shaders.
     */
    class Shader {

    protected:
        /**
         * The logger used with this Shader instance.
         * Defaults to HELIOS_LOG_SCOPE.
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:

        virtual ~Shader() = default;

        /**
         * Activates this shader for subsequent rendering operations.
         * Draw calls of the current rendering pass will use _this_ shader
         * until no or another shader is bound.
         */
        virtual void use() const noexcept = 0;
    };

}