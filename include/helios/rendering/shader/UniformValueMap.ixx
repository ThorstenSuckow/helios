module;

#include <array>
#include <optional>
#include <variant>

export module helios.rendering.shader.UniformValueMap;

import helios.rendering.shader.UniformSemantics;

import helios.math.types;

export namespace helios::rendering::shader {

    /**
     * @brief A type alias for the variant containing all supported
     * uniform types.
     *
     * @see [Str22, 100]
     */
    using UniformValue = std::variant<
        helios::math::mat4f,
        helios::math::vec4f
    >;

    /**
     * @brief Manages a collection of values for mapping them to their UniformSemantics
     * identifier.
     *
     * This class allows for storing different types of uniform values (e.g. mat4f, vec3f, ...)
     * under a single semantic identifier, for convenient assignment to uniform locations
     * of an underlying shader in the rendering process.
     *
     * @note this class goes intentionally without templates as we assume that instances of the
     * value maps are directly used in the rendering hot path.
     *
     * @todo UniformMap must allow only one index for all datastructures,
     * i.e. if mat4fMap_[semantics] is set, vec3fMap_[semantics] must not be allowed
     *
     * @todo UniformValueMaps should be scope, e.g. per frame (world matrix,
     * projection, view, view projection...), per Material (colors, emmissive...),
     * per object (World matrix...) Which allws for better handling of assigning uniforms,
     * e.g. per frame gets changed once, per object gets changed per object etc...
     *
     */
    class UniformValueMap {

    private:
        /**
         * @brief Internal data structure storing the association between a uniform semantic and
         * a concrete value.
         */
        std::array<std::optional<UniformValue>, std::to_underlying(UniformSemantics::count)> map_;

    public:
        ~UniformValueMap() = default;

        UniformValueMap() = default;

        /**
         * @brief Sets or updates a helios::math::mat4f uniform value for a given semantic.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @param mat4f A const ref to the mat4f value to set.
         */
        void set(UniformSemantics uniformSemantics, const helios::math::mat4f& mat4f) noexcept;

        /**
         * @brief Returns a raw const pointer to the mat4f for the specified uniform semantics.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         *
         * @return A raw const pointer to the associated mat4f, or `nullptr` if no mat4f is
         * associated with this semantics.
         */
        [[nodiscard]] const float* mat4f_ptr(UniformSemantics uniformSemantics) const noexcept;

        /**
         * @brief Sets or updates a helios::math::vec4f uniform value for a given semantic.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @param vec4f A const ref to the vec4f value to set.
         */
        void set(UniformSemantics uniformSemantics, const helios::math::vec4f& vec4f) noexcept;

        /**
         * @brief Returns a raw const pointer to the vec4f for the specified uniform semantics.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         *
         * @return A raw const pointer to the associated vec4f, or `nullptr` if no vec4f is
         * associated with this semantics.
         */
        [[nodiscard]] const float* vec4f_ptr(UniformSemantics uniformSemantics) const noexcept;

    };

}