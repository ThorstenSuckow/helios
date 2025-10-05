module;

#include <iostream>

export module helios.ext.opengl.rendering.model.OpenGLShader;

import helios.rendering.model.Shader;

import helios.util.io.StringFileReader;

export namespace helios::ext::opengl::rendering::model {

    /**
     * An OpenGL-specific implementation of a Shader program,
     * consisting of a vertex and a fragment shader.
     *
     * This class manages the lifecycle of the Shaders. Source files are
     * getting loaded via a StringFileReader and immediately  compiled after loading.
     * Any occupied memory for source-files and file-paths to the shader is being cleared
     * once compilation succeeded and are not guaranteed to persist the compilation process.
     */
    class OpenGLShader : public helios::rendering::model::Shader {

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
        void load(
            const std::string& vertexShaderPath,
            const std::string& fragmentShaderPath,
            const helios::util::io::StringFileReader& stringFileReader
        );


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

    public:

        /**
         * Creates and initializes this OpenGLShader.
         * An instance of this class is guaranteed to have a progId_ != 0,
         * hence GLSL-files where successfully loaded and compiled, ready to be used.
         *
         * @param vertexShaderPath The path to the vertex shader.
         * @param fragmentShaderPath The path to the fragment shader.
         * @param stringFileReader The StringFileReader used for loading the shader source files.
         *
         * @throws if creating this shader failed.
         */
        OpenGLShader(
            const std::string& vertexShaderPath,
            const std::string& fragmentShaderPath,
            const helios::util::io::StringFileReader& stringFileReader
        );


        /**
         * Activates this OpenGLShader for subsequent draw calls.
         * This implementation calls `glUseProgram` with the `progId_` received after
         * compilation.
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgram.xhtml
         */
        void use() const noexcept override;

        /**
         * Deletes the program object upon destruction of this instance.
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteProgram.xhtml
         */
        ~OpenGLShader() override;

    };
}