module;

export module helios.rendering.model:Shader;

export namespace helios::rendering::model {

    /**
     * Abstract representation of a Shader program (e.g. composed of vertex/fragment shader).
     *
     * This class defines common operations for managing and utilizing shaders.
     * Derived classes provide rendering API-specifics for loading and compiling the shaders.
     */
    class Shader {

    public:

        virtual ~Shader() = default;

        /**
         * Activates this shader for subsequent rendering operations.
         * Draw calls of the current rendering pass will use _this_ shader
         * until no or another shader is bound.
         */
        virtual void use() const noexcept = 0;
    };

}