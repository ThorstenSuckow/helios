module;

export module helios.rendering.opengl:OpenGLDevice;

import helios.math.types;
import helios.rendering.core.RenderingDevice;

export namespace helios::rendering::opengl {

    class OpenGLDevice : public  core::RenderingDevice {

    public:
        ~OpenGLDevice() override = default;

        bool init() noexcept override;

        void beginRenderPass() const noexcept override;

        void clear() const  noexcept;

        void clearColor(const math::vec4& color) const noexcept;

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
        void setViewport(int x, int y, int width, int height) const noexcept override;



    };
}