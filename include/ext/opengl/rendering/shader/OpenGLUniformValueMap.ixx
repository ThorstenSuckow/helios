module;

#include <map>
#include <variant>

export module helios.ext.opengl.rendering.shader.OpenGLUniformValueMap;

import helios.ext.opengl.rendering.shader.OpenGLUniformSemantics;

import helios.math.types;

export namespace helios::ext::opengl::rendering::shader {

    /**
     * A type alias for the variant containing all supported
     * uniform types.
     *
     * @see [Str13, 100]
     */
    using UniformValue = std::variant<
        helios::math::mat4f
    >;

    /**
     * Manages a collection of values for mapping them to their OpenGLUniformSemantics
     * identifier.
     *
     * This class allows for storing different types of uniform values (e.g. mat4f, vec3f, ...)
     * under a single semantic identifier, for convenient assignment to uniform locations
     * of an underlying shader in the rendering process.
     *
     * @todo UniformMap must allow only one index for all datastructures,
     * i.e. if mat4fMap_[semantics] is set, vec3fMap_[semantics] must not be allowed
     *
     * @note this class goes intentionally without templates as we assume that instances of the
     * value maps are directly used in the rendering hot path.
     */
    class OpenGLUniformValueMap {

    private:
        /**
         * Internal map storing the association between a uniform semantic and
         * a concrete value.
         */
        std::map<OpenGLUniformSemantics, UniformValue> map_;

    public:
        ~OpenGLUniformValueMap() = default;

        OpenGLUniformValueMap() = default;

        /**
         * Sets or updates a helios::math::mat4f uniform value for a given semantic.
         *
         * @param uniformSemantics The `OpenGLUniformSemantics` identifier for the uniform.
         * @param mat4f A const ref to the mat4f value to set.
         */
        void set(OpenGLUniformSemantics uniformSemantics, const helios::math::mat4f& mat4f);

        /**
         * Returns a raw const pointer to the mat4f for the specified uniform semantics.
         *
         * @param uniformSemantics The `OpenGLUniformSemantics` identifier for the uniform.
         *
         * @return A raw const pointer to the associated mat4f, or `nullptr` if no mat4f is
         * associated with this semantics.
         */
        const float* mat4f_ptr(OpenGLUniformSemantics uniformSemantics) const noexcept;

    };

}