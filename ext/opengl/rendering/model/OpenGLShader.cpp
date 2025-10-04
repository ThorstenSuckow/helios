module;

#include <glad/gl.h>
#include <iostream>

module helios.ext.opengl.rendering.model.OpenGLShader;

import helios.rendering.model;
import helios.util.io;

using namespace helios::rendering::model;
using namespace helios::util::io;

namespace helios::ext::opengl::rendering::model {


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
        fragmentShaderSource_.clear();

        logger_.info("Shader loaded and linked");

    }


    void OpenGLShader::use() const noexcept {
        if (!progId_) {
            logger_.error("Cannot use shader, progId_ is invalid");
        }
        glUseProgram(progId_);
    }

};
