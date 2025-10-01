module;

#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

module helios.rendering.opengl;

import helios.util.Log;

using namespace helios::util;

namespace helios::rendering::opengl {

    bool OpenGLDevice::init() noexcept {

        if (initialized_) {
            return true;
        }
        const GLADloadfunc procAddressLoader = glfwGetProcAddress;
        const int gl_ver = gladLoadGL(procAddressLoader);

        if (gl_ver == 0) {
            Log::error("Failed to load OpenGL");
            return false;
        }

        Log::debug(std::format(
            "OpenGL {0}.{1} loaded\n",
            GLAD_VERSION_MAJOR(gl_ver), GLAD_VERSION_MINOR(gl_ver))
        );

        initialized_ = true;

        return true;
    };


    void OpenGLDevice::clear() const noexcept {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLDevice::clearColor(const math::vec4& color) const  noexcept {
        glClearColor(color[0], color[1], color[2], color[3]);
    }

    void OpenGLDevice::beginRenderPass() const noexcept {
        clear();
    };

    void OpenGLDevice::setViewport(const int x, const int y, const int width, const int height) const noexcept {
        glViewport(x, y, width, height);
    }



};
