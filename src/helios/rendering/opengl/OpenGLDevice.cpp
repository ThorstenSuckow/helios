module;

#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <exception>

module helios.rendering.opengl;

import helios.rendering.core;

namespace helios::rendering::opengl {

    OpenGLDevice& OpenGLDevice::init() {

        if (initialized_) {
            return dynamic_cast<OpenGLDevice&>(*this);
        }
        const GLADloadfunc procAddressLoader = glfwGetProcAddress;
        const int gl_ver = gladLoadGL(procAddressLoader);

        if (gl_ver == 0) {
            throw std::exception("Failed to load OpenGL");
        }
        std::printf(  "OpenGL %d.%d loaded\n",  GLAD_VERSION_MAJOR(gl_ver), GLAD_VERSION_MINOR(gl_ver));

        initialized_ = true;

        return dynamic_cast<OpenGLDevice&>(*this);
    };


    OpenGLDevice& OpenGLDevice::setViewport(const int x, const int y, const int width, const int height) {

        glViewport(x, y, width, height);

        return dynamic_cast<OpenGLDevice&>(*this);
    }



};
