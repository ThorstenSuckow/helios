module;

#include <iostream>

export module helios.ext.opengl.rendering.shader.OpenGLShader;

import helios.rendering.shader.Shader;

import helios.util.io.StringFileReader;
import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.rendering.shader.UniformSemantics;
import helios.rendering.shader.UniformValueMap;
import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;


export namespace helios::ext::opengl::rendering::shader {

    /**
     * @brief An OpenGL-specific implementation of a Shader program,
     * consisting of a vertex and a fragment shader.
     *
     * This class manages the lifecycle of the Shaders. Source files are
     * getting loaded via a StringFileReader and immediately  compiled after loading.
     * Any occupied memory for source-files and file-paths to the shader is being cleared
     * once compilation succeeded and are not guaranteed to persist the compilation process.
     */
    class OpenGLShader : public helios::rendering::shader::Shader {

    private:
        /**
         * @brief Source of the shader. Not guaranteed to be persisted
         * once compilation was successful.
         */
        std::string vertexShaderSource_;


        /**
         * @brief Source of the shader. Not guaranteed to be persisted
         * once compilation was successful.
         */
        std::string fragmentShaderSource_;

        /**
         * @brief Loads the specified vertex and fragment shader.
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
         * @brief Compiles the vertex and fragment shader represented by this instance.
         *
         * @return true if compilation succeeded, otherwise false.
         *
         * @throws if compilation failed.
         */
        void compile();


        protected:
            /**
             * @brief The program id as assigned by the underlying rendering backend.
             */
            unsigned int progId_ = 0;

            /**
             * @brief A unique pointer to the OpenGLUniformLocationMap this shader uses.
             */
            std::unique_ptr<const OpenGLUniformLocationMap> uniformLocationMap_ = nullptr;

    public:
        /**
         * @brief Rule of three.
         * @see https://wikis.khronos.org/opengl/Common_Mistakes#RAII_and_hidden_destructor_calls
         * @see https://en.cppreference.com/w/cpp/language/rule_of_three.html
         *
         * Prevent copying.
         */
        OpenGLShader(const OpenGLShader&)=delete;
        OpenGLShader& operator =(const OpenGLShader&) = delete;


        /**
         * @brief Creates and initializes this OpenGLShader.
         * An instance of this class is guaranteed to have a progId_ != 0,
         * hence shader-files where successfully loaded and compiled, ready to be used.
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
         * @brief Activates this OpenGLShader for subsequent draw calls.
         * This implementation calls `glUseProgram` with the `progId_` received after
         * compilation.
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgram.xhtml
         */
        void use() const noexcept override;

        /**
         * @brief Deletes the program object upon destruction of this instance.
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteProgram.xhtml
         */
        ~OpenGLShader() override;

        /**
         * @brief Sets the OpenGLUniformLocationMap for this OpenGLShader.
         * Ownership is transferred to this instance.
         *
         * @param uniformLocationMap The OpenGLUniformMap providing the mappings for the uniforms
         * of the underlying GLSL shader.
         */
        void setUniformLocationMap(
            std::unique_ptr<const OpenGLUniformLocationMap> uniformLocationMap) noexcept;

        /**
         * @brief Returns the uniform location for the uniform represented by the specified
         * UniformSemantics.
         *
         * @param uniformSemantics The `UniformSemantics` identifier.
         *
         * @return The integer representing the uniform variable location in the shader, or
         * -1 if no location map was registered with this shader or if the uniform with the
         * specified semantics was not found.
         */
        [[nodiscard]] int uniformLocation(
            helios::rendering::shader::UniformSemantics uniformSemantics) const noexcept;

        /**
         * @copydoc helios::rendering::shader::Shader::applyUniformValues()
         */
        void applyUniformValues(
            const helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept override;

    };
}