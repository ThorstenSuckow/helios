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


        /**
         * Wrapper for glViewport
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @return
         */
        OpenGLDevice& setViewport(const int x, const int y, const int width, const int height) override {

            glViewport(x, y, width, height);

            return dynamic_cast<OpenGLDevice&>(*this);
        }



    };
}