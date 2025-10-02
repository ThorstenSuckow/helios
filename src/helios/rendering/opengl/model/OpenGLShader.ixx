module;

#include <iostream>

export module helios.rendering.opengl.model:OpenGLShader;

import helios.rendering.model;

export namespace helios::rendering::opengl::model {
    class OpenGLShader : public rendering::model::Shader {

    private:
        /**
         * Source of the shader. Not guaranteed to be persisted
         * once compilation was successful.
         */
        std::string vertexShaderSource_ = "";


        /**
         * Source of the shader. Not guaranteed to be persisted
         * once compilation was successful.
         */
        std::string fragmentShaderSource_ = "";

        /**
         * Loads the specified vertex and fragment shader.
         *
         * @return true if loading succeeded, otherwise false.
         *
         * @throws if loading the specified files failed.
         */
        void load();


        /**
         * Compiles the vertex and fragment shader represented by this instance.
         *
         * @return true if compilation succeeded, otherwise false.
         *
         * @throws if compilation failed.
         */
        void compile();


        protected:
            unsigned int progId_ = 0;
            const std::string vertexShaderPath_;
            const std::string fragmentShaderPath_;


    public:

        /**
         * Creates and initializes this OpenGLShader.
         * An instance of this class is guaranteed to have a progId_ != 0,
         * hence GLSL-files where successfully loaded and compiled, ready to be used.
         *
         * @param vertexShaderPath
         * @param fragmentShaderPath
         *
         * @throws if creating this shader failed.
         */
        OpenGLShader(const std::string& vertexShaderPath,  const std::string& fragmentShaderPath);


        void use() const noexcept override;

    };
}