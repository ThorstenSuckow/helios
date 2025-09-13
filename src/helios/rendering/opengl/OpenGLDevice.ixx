module;

#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <exception>

export module helios.rendering.opengl:OpenGLDevice;

import helios.rendering.core;

export namespace helios::rendering::opengl {

    class OpenGLDevice : public  core::RenderingDevice {

    public:
        ~OpenGLDevice() override = default;


        OpenGLDevice& init() override {

            const GLADloadfunc procAddressLoader = glfwGetProcAddress;
            const int gl_ver = gladLoadGL(procAddressLoader);
            if (gl_ver == 0) {
                throw std::exception("Failed to load OpenGL");
            }
            std::printf(  "OpenGL %d.%d loaded\n",  GLAD_VERSION_MAJOR(gl_ver), GLAD_VERSION_MINOR(gl_ver));
            glViewport(0,0,800, 600);

            return dynamic_cast<OpenGLDevice&>(*this);
        };
    };
}