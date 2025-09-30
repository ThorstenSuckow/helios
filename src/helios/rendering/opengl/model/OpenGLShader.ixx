module;

#include <iostream>

export module helios.rendering.opengl.model:OpenGLShader;

import helios.rendering.model;

using namespace helios::rendering::model;

export namespace helios::rendering::opengl::model {
    class OpenGLShader : public Shader {

    protected:
        unsigned int progId_ = 0;
        std::string vertexShaderSource_ = "";
        std::string fragmentShaderSource_ = "";

        const std::string vertexShaderPath_;
        const std::string fragmentShaderPath_;


    public:

        OpenGLShader(const std::string& vertexShaderPath,  const std::string& fragmentShaderPath);

        bool load() noexcept override;

        bool compile() noexcept override;

        void use() const noexcept override;

    };
}