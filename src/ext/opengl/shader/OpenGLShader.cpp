module;

#include <glad/gl.h>
#include <format>
#include <helios/helios_config.h>

module helios.ext.opengl.rendering.shader.OpenGLShader;

import helios.rendering.model;
import helios.util.io;

import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.rendering.shader.UniformSemantics;
import helios.rendering.shader.UniformValueMap;

using namespace helios::rendering::shader;
using namespace helios::rendering::model;
using namespace helios::util::io;

namespace helios::ext::opengl::rendering::shader {


    OpenGLShader::OpenGLShader(
        const std::string& vertexShaderPath,
        const std::string& fragmentShaderPath,
        const StringFileReader& stringFileReader
    ) {

        try{
            load(vertexShaderPath, fragmentShaderPath, stringFileReader);
            compile();
        } catch (std::runtime_error& e) {
            logger_.error("Could not initialize shader");
            throw std::runtime_error("Could not initialize shader");
        }
    }


    void OpenGLShader::load(
        const std::string& vertexShaderPath,
        const std::string& fragmentShaderPath,
        const StringFileReader& stringFileReader)
    {
        logger_.info(std::format("Loading shader from {0}, {1}", vertexShaderPath, fragmentShaderPath));
        if (!stringFileReader.readInto(fragmentShaderPath, fragmentShaderSource_) ||
            !stringFileReader.readInto(vertexShaderPath, vertexShaderSource_)) {
            logger_.error("Could not load shader");
            throw std::runtime_error("Could not load shader");
        }
    }


     void OpenGLShader::compile() {

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


    void OpenGLShader::use() const noexcept {
        if (!progId_) {
            logger_.error("Cannot use shader, progId_ is invalid");
        }
        glUseProgram(progId_);
    }


    OpenGLShader::~OpenGLShader() {
        if (progId_ != 0) {
            glDeleteProgram(progId_);
        }
    }

    void OpenGLShader::setUniformLocationMap(
        std::unique_ptr<const OpenGLUniformLocationMap> uniformLocationMap) noexcept {
        uniformLocationMap_ = std::move(uniformLocationMap);
    }

    int OpenGLShader::uniformLocation(const helios::rendering::shader::UniformSemantics uniformSemantics) const noexcept {
        if (uniformLocationMap_) {
            return uniformLocationMap_->get(uniformSemantics);
        }

        return -1;
    }

    void OpenGLShader::applyUniformValues(
        const UniformValueMap& uniformValueMap) const noexcept {

        logger_.info("Applying uniform values");

        #if HELIOS_DEBUG
        if (static_cast<size_t>(UniformSemantics::size_) > 1) {
            logger_.warn(
                std::format(
                    "UniformSemantics has more entries ({0}) than considered in OpenGLShader::applyUniformSemantics",
                    static_cast<size_t>(UniformSemantics::size_)
                )
            );
        }
        #endif

        if (const auto worldMatrixUniform = uniformLocation(UniformSemantics::WorldMatrix); worldMatrixUniform != -1) {
            if (const auto* mat4f_ptr = uniformValueMap.mat4f_ptr(UniformSemantics::WorldMatrix)) {
                glUniformMatrix4fv(worldMatrixUniform, 1, false, mat4f_ptr);
            }
        }
        if (const auto materialBaseColorUniform = uniformLocation(UniformSemantics::MaterialBaseColor); materialBaseColorUniform != -1) {
            if (const auto* vec4f_ptr = uniformValueMap.vec4f_ptr(UniformSemantics::MaterialBaseColor)) {
                glUniform4fv(materialBaseColorUniform, 1, vec4f_ptr);
            }
        }
    }

};
