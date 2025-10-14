module;


#include <map>

export module helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;

import helios.rendering.shader.UniformSemantics;
import helios.util.log.LogManager;
import helios.util.log.Logger;

#define HELIOS_LOG_SCOPE "helios::ext::opengl::rendering::shader::OpenGLUniformLocationMap"
export namespace helios::ext::opengl::rendering::shader {

    /**
     * Manages the mapping between OpenGL uniform semantics and their locations
     * in an underlying OpenGL shader.
     *
     * This class should be used to store and retrieve the uniform locations, e.g.
     * for a OpenGL render backend for easy access.
     */
    class OpenGLUniformLocationMap final {

    private:
        /**
         * The internal map used for mapping uniform semantics to positions.
         */
        std::map<helios::rendering::shader::UniformSemantics, int> map_;

        /**
         * The logger used with this OpenGLUniformLocationMap.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:
        ~OpenGLUniformLocationMap() = default;

        OpenGLUniformLocationMap() = default;

        /**
         * Sets or updates the OpenGLUniform.
         * If the semantics already exists in the map, it is updated with `position`.
         *
         * @param uniformSemantics The uniformSemantics to register with `position`
         * @param position A positive integer value as the location of the uniform represented by
         * uniformSemantics.
         *
         * @return true if the position for the specified uniformSemantics was registered with this
         * map, otherwise false (e.g. if position was < 0)
         */
        [[nodiscard]] bool set(
            helios::rendering::shader::UniformSemantics uniformSemantics,
            int position
        ) noexcept;

        /**
         * Returns the index of the uniform variable for the specified UniformSemantics
         * as configured with this location map.
         * This method returns -1 if the specified semantics is not configured with this map.
         *
         * @param uniformSemantics The UniformSemantics for which the location index
         * should be returned.
         *
         * @return The index as previously set for the specified uniformSemantics, or -1
         * if not found.
         *
         * @see glGetUniformLocation
         * @see [KSS17, 47]
         */
        [[nodiscard]] int get(helios::rendering::shader::UniformSemantics uniformSemantics) const noexcept;
    };


}