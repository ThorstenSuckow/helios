/**
 * @file UniformValueMap.ixx
 * @brief Mapping of uniform names to values used by shader programs.
 */
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
        helios::math::vec4f,
        float,
        int
    >;

    /**
     * @brief Maps uniform semantics to their values for shader parameter binding.
     *
     * `UniformValueMap` stores uniform values (matrices, vectors, scalars) indexed by
     * their `UniformSemantics` identifier. This allows the rendering pipeline to
     * efficiently pass data to shaders without string-based lookups.
     *
     * ## Supported Types
     *
     * - `mat4f` – 4x4 transformation matrices
     * - `vec4f` – 4-component vectors (colors, positions)
     * - `float` – Scalar values (roughness, time)
     * - `int` – Integer values (texture units)
     *
     * ## Usage
     *
     * ```cpp
     * UniformValueMap uniforms;
     *
     * // Set transform matrices
     * uniforms.set(UniformSemantics::ModelMatrix, modelMat);
     * uniforms.set(UniformSemantics::ViewMatrix, viewMat);
     *
     * // Set material properties
     * uniforms.set(UniformSemantics::MaterialBaseColor, vec4f{1.0f, 0.0f, 0.0f, 1.0f});
     *
     * // Retrieve raw pointer for OpenGL
     * const float* modelPtr = uniforms.mat4f_ptr(UniformSemantics::ModelMatrix);
     * ```
     *
     * @todo UniformMap must allow only one index for all data structures,
     * i.e. if map_[semantics] contains a mat4f, other types should not be allowed
     * for the same semantics.
     *
     * @todo UniformValueMaps should be scoped, e.g. per frame (world matrix,
     * projection, view...), per material (colors, emissive...), per object (world matrix...),
     * which allows for better handling of assigning uniforms: per-frame values change once per frame,
     * while per-object values change per object.
     */
    class UniformValueMap {

    private:
        /**
         * @brief Internal data structure storing the association between a uniform semantic and
         * a concrete value.
         */
        std::array<std::optional<UniformValue>, std::to_underlying(UniformSemantics::size_)> map_;

    public:
        ~UniformValueMap() = default;

        UniformValueMap() = default;

        /**
         * @brief Sets or updates a `helios::math::mat4f` uniform value for a given semantic.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @param mat4f A const ref to the mat4f value to set.
         */
        void set(UniformSemantics uniformSemantics, const helios::math::mat4f& mat4f) noexcept {
            map_[std::to_underlying(uniformSemantics)].emplace(mat4f);
        }

        /**
         * @brief Returns a raw const pointer to the `mat4f` for the specified uniform semantics.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @return A raw const pointer to the associated mat4f, or `nullptr` if no mat4f is
         * associated with this semantics.
         */
        [[nodiscard]] const float* mat4f_ptr(UniformSemantics uniformSemantics) const noexcept {

            const auto index = std::to_underlying(uniformSemantics);

            if (index >= map_.size()) {
                return nullptr;
            }

            if (const auto& el = map_[index]; el.has_value()) {
                if (const auto* it = std::get_if<helios::math::mat4f>(&el.value())) {
                    return helios::math::value_ptr(*it);
                }
            }

            return nullptr;
        }

        /**
         * @brief Sets or updates a `helios::math::vec4f` uniform value for a given semantic.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @param vec4f A const ref to the vec4f value to set.
         */
        void set(UniformSemantics uniformSemantics, const helios::math::vec4f& vec4f) noexcept {
            map_[std::to_underlying(uniformSemantics)].emplace(vec4f);
        }

        /**
         * @brief Returns a raw const pointer to the `vec4f` for the specified uniform semantics.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @return A raw const pointer to the associated vec4f, or `nullptr` if no vec4f is
         * associated with this semantics.
         */
        [[nodiscard]] const float* vec4f_ptr(UniformSemantics uniformSemantics) const noexcept {

            const auto index = std::to_underlying(uniformSemantics);

            if (index >= map_.size()) {
                return nullptr;
            }

            if (const auto& el = map_[index]; el.has_value()) {
                if (const auto* it = std::get_if<helios::math::vec4f>(&el.value())) {
                    return helios::math::value_ptr(*it);
                }
            }

            return nullptr;
        }

        /**
         * @brief Sets or updates an integer uniform value for a given semantic.
         *
         * Typically used for texture unit indices.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @param value The integer value to set.
         */
        void set(UniformSemantics uniformSemantics, int value) noexcept {
            map_[std::to_underlying(uniformSemantics)].emplace(value);
        }

        /**
         * @brief Sets or updates a float uniform value for a given semantic.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         * @param value The float value to set.
         */
        void set(UniformSemantics uniformSemantics, float value) noexcept {
            map_[std::to_underlying(uniformSemantics)].emplace(value);
        }

        /**
         * @brief Returns a raw const pointer to the float value for the specified uniform semantics.
         *
         * This method retrieves the float value associated with a given `UniformSemantics` identifier.
         * If no float value is associated with the specified semantics, the method returns `nullptr`.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         *
         * @return A raw const pointer to the associated float value, or `nullptr` if no float value
         * is associated with this semantics.
         */
        [[nodiscard]] const float* float_ptr(UniformSemantics uniformSemantics) const noexcept {

            const auto index = std::to_underlying(uniformSemantics);

            if (index >= map_.size()) {
                return nullptr;
            }

            if (const auto& el = map_[index]; el.has_value()) {
                if (const auto* floatPtr = std::get_if<float>(&el.value())) {
                    return floatPtr;
                }
            }

            return nullptr;
        }

        /**
         * @brief Returns a raw const pointer to the integer value for the specified uniform semantics.
         *
         * @param uniformSemantics The `UniformSemantics` identifier for the uniform.
         *
         * @return A raw const pointer to the associated integer value, or `nullptr` if no integer
         *         value is associated with this semantics.
         */
        [[nodiscard]] const int* int_ptr(UniformSemantics uniformSemantics) const noexcept {

            const auto index = std::to_underlying(uniformSemantics);

            if (index >= map_.size()) {
                return nullptr;
            }

            if (const auto& el = map_[index]; el.has_value()) {
                if (const auto* intPtr = std::get_if<int>(&el.value())) {
                    return intPtr;
                }
            }

            return nullptr;
        }

    };

}