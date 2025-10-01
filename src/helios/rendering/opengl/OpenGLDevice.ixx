module;

export module helios.rendering.opengl:OpenGLDevice;

import helios.math.types;
import helios.rendering.core.RenderingDevice;

export namespace helios::rendering::opengl {

    class OpenGLDevice : public  core::RenderingDevice {

    public:
        ~OpenGLDevice() override = default;

        void init() noexcept override;

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
        void clearColor(const math::vec4& color) const noexcept;

    };
}