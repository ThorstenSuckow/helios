module;

export module helios.ext.opengl.rendering.OpenGLDevice;

import helios.math.types;
import helios.rendering.RenderingDevice;

export namespace helios::ext::opengl::rendering {

    /**
     * OpenGL RenderingDevice representative.
     */
    class OpenGLDevice : public  helios::rendering::RenderingDevice {

    public:
        ~OpenGLDevice() override = default;

        void init() override;

        void beginRenderPass() const noexcept override;

        void setViewport(int x, int y, int width, int height) const noexcept override;

        /**
         * Clears the color buffer to preset values.
         *
         * @see clearColor
         */
        void clear() const  noexcept;

        /**
         * Specifies the RGBa values clamped to [0, 1] when color
         * buffers are cleared.
         *
         * @param color
         *
         * @see clear
         */
        void clearColor(const math::vec4f& color) const noexcept;

    };
}