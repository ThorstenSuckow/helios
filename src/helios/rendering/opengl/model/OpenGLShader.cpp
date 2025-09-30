module helios.rendering.opengl.model:OpenGLShader;

#include <glad/gl.h>
#include <iostream>
#include <stdexcept>


import helios.rendering.model;
import helios.util.IOUtil;
import helios.util.Log;


using namespace helios::rendering::model;
using namespace helios::util;

namespace helios::rendering::opengl::model {

    OpenGLShader::OpenGLShader(
        const std::string& vertexShaderPath,
        const std::string& fragmentShaderPath) :
        vertexShaderPath_(vertexShaderPath),
        fragmentShaderPath_(fragmentShaderPath) {
            OpenGLShader::load();
        }


    bool OpenGLShader::load() noexcept override {

        if (!vertexShaderSource_.empty()  || !fragmentShaderSource_.empty()) {
            Log::warn("Shader already loaded");
            return false;
        }

        if (!IOUtil::readInto(fragmentShaderPath_, fragmentShaderSource_) ||
            !IOUtil::readInto(vertexShaderPath_, vertexShaderSource_)) {
            Log::error("Could not load shader");
            return false;
        }

        return true;
    }


     bool OpenGLShader::compile() noexcept override {
        if (progId_ != 0) {
            return true;
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

            Log::error("VERTEX::COMPILATION_FAILED\n" + static_cast<std::string>(infoLog));
            return false;
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            Log::error("SHADER::FRAGMENT::COMPILATION_FAILED\n" + static_cast<std::string>(infoLog));
            return false;
        }

        progId_ = glCreateProgram();

        glAttachShader(progId_, vertexShader);
        glAttachShader(progId_, fragmentShader);
        glLinkProgram(progId_);

        glGetProgramiv(progId_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(progId_, 512, nullptr, infoLog);
            Log::error("PROGRAM_LINKING_FAILED\n" + static_cast<std::string>(infoLog));
            return false;
        }


        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return true;
    }


    void OpenGLShader::use() const noexcept override {
        glUseProgram(progId_);
    }

};
