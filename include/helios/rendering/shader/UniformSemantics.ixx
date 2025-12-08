/**
 * @file UniformSemantics.ixx
 * @brief Definitions of uniform semantics used by shaders.
 */
module;

export module helios.rendering.shader.UniformSemantics;


export namespace helios::rendering::shader {

    /**
     * @brief Enumeration class defining common semantic meanings for uniform variables in
     * Shaders.
     *
     * This enum class provides a strongly typed way to refer to **standard** uniform
     * variables that represent essential matrices or other global data typically passed
     * to shaders.
     *
     * Concrete shader implementations use these semantics to map actual uniform names
     * and their respective uniform locations.
     *
     * @see UniformValueMap
     */
    enum class UniformSemantics {

        /**
         * @brief Represents the world transformation matrix of an object.
         */
        WorldMatrix = 0,

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