module;


export module helios.ext.opengl.rendering.shader.OpenGLUniformSemantics;

export namespace helios::ext::opengl::rendering::shader {

    /**
     * Enumeration class defining common semantic meanings for uniform variables in
     * OpenGLShader.
     *
     * This enum class provides a strongly typed way to refer to **standard** uniform
     * variables that represent essential matrices or other global data typically passed
     * to OpenGL shaders.
     *
     * Concrete OpenGL shader implementations use these semantics to map actual uniform names
     * and their respective uniform locations.
     *
     * @see OpenGLUniformLocationMap
     * @see OpenGLUniformValueMap
     */
    enum class OpenGLUniformSemantics {

        /**
         * Represents the world transformation matrix of an object.
         */
        WORLD_MATRIX,

        /**
         * Represents the projection matrix.
         */
        PROJECTION_MATRIX,

        /**
         * Represents the (camera) view space, transforming world space to view space.
         */
        VIEW_MATRIX
    };
}