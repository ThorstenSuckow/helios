/**
 * @file IsShaderLike.ixx
 * @brief Concept constraining types usable as shader abstractions.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsShaderLike;

import helios.rendering.shader.UniformValueMap;

using helios::rendering::shader::UniformValueMap;

export namespace helios::engine::common::concepts {

    /**
     * @brief Constrains T to types that provide a minimal shader interface.
     *
     * @details A type satisfies IsShaderLike if it:
     * 1. Provides `void use()` to bind the shader for rendering.
     * 2. Provides `void applyUniformValues(UniformValueMap&)` to set
     *    uniform parameters from a value map.
     *
     * @tparam T The type to constrain.
     *
     * @see UniformValueMap
     */
    template<class T>
    concept IsShaderLike = requires(T& t, UniformValueMap& uniformValueMap) {
        {
            t.use()
        } -> std::same_as<void>;
        {
            t.applyUniformValues(uniformValueMap)
        } -> std::same_as<void>;

    };

}