module;

#include <array>
#include <optional>
#include <variant>

export module helios.rendering.shader.UniformValueMap;

import helios.rendering.shader.UniformSemantics;

import helios.math.types;

export namespace helios::rendering::shader {

    /**
     * A type alias for the variant containing all supported
     * uniform types.
     *
     * @see [Str22, 100]
     */
    using UniformValue = std::variant<
        helios::math::mat4f
    >;

    /**
     * Manages a collection of values for mapping them to their UniformSemantics
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
    class UniformValueMap {

    private:
        /**
         * Internal data structure storing the association between a uniform semantic and
         * a concrete value.
         */
        std::array<std::optional<UniformValue>, std::to_underlying(UniformSemantics::count)> map_;

    public:
        ~UniformValueMap() = default;

        UniformValueMap() = default;

        /**
         * Sets or updates a helios::math::mat4f uniform value for a given semantic.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @param mat4f A const ref to the mat4f value to set.
         */
        void set(UniformSemantics uniformSemantics, const helios::math::mat4f& mat4f) noexcept;

        /**
         * Returns a raw const pointer to the mat4f for the specified uniform semantics.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         *
         * @return A raw const pointer to the associated mat4f, or `nullptr` if no mat4f is
         * associated with this semantics.
         */
        [[nodiscard]] const float* mat4f_ptr(UniformSemantics uniformSemantics) const noexcept;

    };

}