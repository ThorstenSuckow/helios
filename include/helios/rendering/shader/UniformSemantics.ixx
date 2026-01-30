/**
 * @file UniformSemantics.ixx
 * @brief Definitions of uniform semantics used by shaders.
 */
module;

export module helios.rendering.shader.UniformSemantics;


export namespace helios::rendering::shader {

    /**
     * @brief Enumeration of semantic identifiers for shader uniform variables.
     *
     * `UniformSemantics` provides strongly-typed identifiers for standard uniform
     * variables passed to shaders. These semantics abstract the actual uniform names,
     * allowing the rendering pipeline to work with meaningful identifiers.
     *
     * ## Categories
     *
     * - **Transform Matrices:** `ModelMatrix`, `ViewMatrix`, `ProjectionMatrix`
     * - **Material Properties:** `MaterialBaseColor`, `MaterialRoughness`
     * - **Text Rendering:** `TextTexture`, `TextColor`
     * - **Time Values:** `deltaTime`, `totalTime`
     *
     * ## Usage
     *
     * ```cpp
     * UniformValueMap uniforms;
     * uniforms.set(UniformSemantics::ModelMatrix, modelMat);
     * uniforms.set(UniformSemantics::MaterialBaseColor, vec4f{1.0f, 0.0f, 0.0f, 1.0f});
     * ```
     *
     * @see UniformValueMap
     */
    enum class UniformSemantics {

        /**
         * @brief Represents the object-to-world transformation matrix of an object.
         */
        ModelMatrix = 0,

        /**
         * @brief Represents the projection matrix.
         */
        ProjectionMatrix,

        /**
         * @brief Represents the (camera) view space, transforming world space to view space.
         */
        ViewMatrix,

        /**
         * @brief Represents the (material's) base color.
         */
        MaterialBaseColor,

        /**
         * @brief Represents the (material's) roughness factor.
         */
        MaterialRoughness,

        /**
         * @brief Represents the texture sampler for text glyph rendering.
         */
        TextTexture,

        /**
         * @brief Represents the text color for glyph rendering.
         */
        TextColor,

        /**
         * @brief Represents the time elapsed since the last frame.
         */
        deltaTime,

        /**
         * @brief Represents the total time elapsed since measuring frame times.
         */
        totalTime,

        /**
         * @brief Number of entries in this enum.
         */
        size_
    };


}