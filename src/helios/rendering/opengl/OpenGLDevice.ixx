module;

export module helios.rendering.opengl:OpenGLDevice;

import helios.rendering.core;

export namespace helios::rendering::opengl {

    class OpenGLDevice : public  core::RenderingDevice {


    public:
        ~OpenGLDevice() override = default;


        OpenGLDevice& init() override;


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
        OpenGLDevice& setViewport(int x, int y, int width, int height) override;



    };
}