/**
 * @file OpenGLShader.ixx
 * @brief OpenGL implementation of shader program management.
 */
module;

#include <glad/gl.h>
#include <format>
#include <stdexcept>
#include <string>
#include <helios/helios_config.h>

export module helios.ext.opengl.rendering.shader.OpenGLShader;

import helios.rendering.shader.Shader;

import helios.util.io.StringFileReader;
import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.rendering.shader.UniformSemantics;
import helios.rendering.shader.UniformValueMap;


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
        ) {
            logger_.info(std::format("Loading shader from {0}, {1}", vertexShaderPath, fragmentShaderPath));
            if (!stringFileReader.readInto(fragmentShaderPath, fragmentShaderSource_) ||
                !stringFileReader.readInto(vertexShaderPath, vertexShaderSource_)) {
                logger_.error("Could not load shader");
                throw std::runtime_error("Could not load shader");
            }
        }


        /**
         * @brief Compiles the vertex and fragment shader represented by this instance.
         *
         * @return true if compilation succeeded, otherwise false.
         *
         * @throws if compilation failed.
         */
        void compile() {
            if (progId_ != 0) {
                logger_.warn("Shader already compiled");
                return;
            }

            const GLchar* vertexSrc = vertexShaderSource_.c_str();
            const GLchar* fragmentSrc = fragmentShaderSource_.c_str();

            const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
            glCompileShader(vertexShader);

            const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
            glCompileShader(fragmentShader);


            int success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

            if (!success) {
                glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

                logger_.error("VERTEX::COMPILATION_FAILED " + static_cast<std::string>(infoLog));
                throw std::runtime_error("Vertex Shader Compilation failed.");
            }

            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
                logger_.error("SHADER::FRAGMENT::COMPILATION_FAILED " + static_cast<std::string>(infoLog));
                throw std::runtime_error("Fragment Shader Compilation failed.");
            }

            progId_ = glCreateProgram();

            glAttachShader(progId_, vertexShader);
            glAttachShader(progId_, fragmentShader);
            glLinkProgram(progId_);

            glGetProgramiv(progId_, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(progId_, 512, nullptr, infoLog);
                logger_.error("PROGRAM_LINKING_FAILED " + static_cast<std::string>(infoLog));
                throw std::runtime_error("Program linking failed.");
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            vertexShaderSource_.clear();
            vertexShaderSource_.shrink_to_fit();
            fragmentShaderSource_.clear();
            fragmentShaderSource_.shrink_to_fit();

            logger_.info("Shader loaded and linked");
        }


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
        ) {
            try {
                load(vertexShaderPath, fragmentShaderPath, stringFileReader);
                compile();
            } catch (std::runtime_error& e) {
                logger_.error("Could not initialize shader");
                throw std::runtime_error("Could not initialize shader");
            }
        }


        /**
         * @brief Activates this OpenGLShader for subsequent draw calls.
         * This implementation calls `glUseProgram` with the `progId_` received after
         * compilation.
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUseProgram.xhtml
         */
        void use() const noexcept override {
            if (!progId_) {
                logger_.error("Cannot use shader, progId_ is invalid");
            }
            glUseProgram(progId_);
        }

        /**
         * @brief Deletes the program object upon destruction of this instance.
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteProgram.xhtml
         */
        ~OpenGLShader() override {
            if (progId_ != 0) {
                glDeleteProgram(progId_);
            }
        }

        /**
         * @brief Sets the OpenGLUniformLocationMap for this OpenGLShader.
         * Ownership is transferred to this instance.
         *
         * @param uniformLocationMap The OpenGLUniformMap providing the mappings for the uniforms
         * of the underlying GLSL shader.
         */
        void setUniformLocationMap(
            std::unique_ptr<const OpenGLUniformLocationMap> uniformLocationMap) noexcept {
            uniformLocationMap_ = std::move(uniformLocationMap);
        }

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
            helios::rendering::shader::UniformSemantics uniformSemantics) const noexcept {
            if (uniformLocationMap_) {
                return uniformLocationMap_->get(uniformSemantics);
            }

            return -1;
        }

        /**
         * @copydoc helios::rendering::shader::Shader::applyUniformValues()
         */
        void applyUniformValues(
            const helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept override {

            logger_.info("Applying uniform values");

            #ifdef HELIOS_DEBUG
            if (static_cast<size_t>(helios::rendering::shader::UniformSemantics::size_) > 1) {
                logger_.warn(
                    std::format(
                        "UniformSemantics has more entries ({0}) than considered in OpenGLShader::applyUniformSemantics",
                        static_cast<size_t>(helios::rendering::shader::UniformSemantics::size_)
                    )
                );
            }
            #endif

            if (const auto viewMatrixUniform = uniformLocation(helios::rendering::shader::UniformSemantics::ViewMatrix); viewMatrixUniform != -1) {
                if (const auto* mat4f_ptr = uniformValueMap.mat4f_ptr(helios::rendering::shader::UniformSemantics::ViewMatrix)) {
                    glUniformMatrix4fv(viewMatrixUniform, 1, false, mat4f_ptr);
                }
            }

            if (const auto projectionMatrixUniform = uniformLocation(helios::rendering::shader::UniformSemantics::ProjectionMatrix); projectionMatrixUniform != -1) {
                if (const auto* mat4f_ptr = uniformValueMap.mat4f_ptr(helios::rendering::shader::UniformSemantics::ProjectionMatrix)) {
                    glUniformMatrix4fv(projectionMatrixUniform, 1, false, mat4f_ptr);
                }
            }

            if (const auto modelMatrixUniform = uniformLocation(helios::rendering::shader::UniformSemantics::ModelMatrix); modelMatrixUniform != -1) {
                if (const auto* mat4f_ptr = uniformValueMap.mat4f_ptr(helios::rendering::shader::UniformSemantics::ModelMatrix)) {
                    glUniformMatrix4fv(modelMatrixUniform, 1, false, mat4f_ptr);
                }
            }
            if (const auto materialBaseColorUniform = uniformLocation(helios::rendering::shader::UniformSemantics::MaterialBaseColor); materialBaseColorUniform != -1) {
                if (const auto* vec4f_ptr = uniformValueMap.vec4f_ptr(helios::rendering::shader::UniformSemantics::MaterialBaseColor)) {
                    glUniform4fv(materialBaseColorUniform, 1, vec4f_ptr);
                }
            }
        }

    };
}