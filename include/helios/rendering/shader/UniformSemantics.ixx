module;


export module helios.rendering.shader.UniformSemantics;

export namespace helios::rendering::shader {

    /**
     * Enumeration class defining common semantic meanings for uniform variables in
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
         * Represents the world transformation matrix of an object.
         */
        WorldMatrix,

        /**
         * Represents the projection matrix.
         */
        ProjectionMatrix,

        /**
         * Represents the (camera) view space, transforming world space to view space.
         */
        ViewMatrix
    };
}