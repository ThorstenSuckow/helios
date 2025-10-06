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

        /**
         * Initializes the OpenGL device to access modern OpenGL.
         * This method must be called **after** the helios-Application's
         * setCurrent() was called for creating a current context. This
         * OpenGL-Device will then load from this context.
         * This method will use the `gladLoadGl`-method to in conjunction
         * with `glfwGetProcAddress`, which returns the address of the specific
         * OpenGL core.
         *
         * @see https://www.glfw.org/docs/latest/group__context.html#ga35f1837e6f666781842483937612f163
         *
         * @todo provide abstraction for glfwGetProcAddress
         */
        void init() override;


        /**
         * Begins a new render pass.
         * This implementation makes sure that the rendering surface is cleared
         * with the current clear-color, which can be configured via clearColor().-
         *
         * @see clear()
         * @see clearColor();
         */
        void beginRenderPass() const noexcept override;


        /**
         * @copydoc helios::rendering::RenderingDevice::setViewport()
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml
         */
        void setViewport(const int x, const int y, const int width, const int height) const noexcept override;


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