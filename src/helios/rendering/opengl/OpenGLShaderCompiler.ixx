module;

#include <glad/gl.h>
#include <string>
#include <iostream>

export module helios.rendering.opengl:OpenGLShaderCompiler;


export namespace helios::rendering::opengl {

    /**
     *
     *
     * SWH15, Listing 2.5, 19
     */
    class OpenGLShaderCompiler {


    public:
        static int compile(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource) {

            const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
            glCompileShader(vertexShader);

            const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
            glCompileShader(fragmentShader);


            int success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

            if (!success) {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                throw std::runtime_error(
                    "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + static_cast<std::string>(infoLog));
            }

            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
                throw std::runtime_error(
                    "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + static_cast<std::string>(infoLog));
            }

            const unsigned int shaderProgram = glCreateProgram();

            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
                throw std::runtime_error(
                    "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + static_cast<std::string>(infoLog));
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            return shaderProgram;
        }

    };


}