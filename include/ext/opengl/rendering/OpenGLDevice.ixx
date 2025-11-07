module;

#include <glad/gl.h>

export module helios.ext.opengl.rendering.OpenGLDevice;

import helios.math.types;
import helios.rendering.RenderingDevice;
import helios.rendering.RenderQueue;
import helios.rendering.RenderPass;
import helios.rendering.model.config.PrimitiveTopology;

export namespace helios::ext::opengl::rendering {

    /**
     * @brief OpenGL RenderingDevice representative.
     */
    class OpenGLDevice : public  helios::rendering::RenderingDevice {

        /**
         * @brief Translates helios abstract PrimitiveTopology enum to its corresponding
         * OpenGL GLenum value.
         *
         * This ia a utility function used exclusively by the OpenGLDevice to interpret the
         * API-agnostic primitive type requested by the helios' configuration.
         *
         * @param primitiveTopology The API-agnostic PrimitiveTopology value.
         *
         * @return The corresponding OpenGL primitive type as a GLenum. If mapping didn't
         * succeed, the function falls back to GL_TRIANGLES.
         */
        [[nodiscard]] GLenum toOpenGL(helios::rendering::model::config::PrimitiveTopology primitiveTopology) const noexcept;


    public:
        ~OpenGLDevice() override = default;

        /**
         * @brief Initializes the OpenGL device to access modern OpenGL.
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
         * @brief Begins a new render pass.
         * This implementation makes sure that the rendering surface is cleared
         * with the current clear-color, which can be configured via clearColor().-
         *
         * @see clear()
         * @see clearColor();
         */
        void beginRenderPass(helios::rendering::RenderPass& renderPass) const noexcept override;

        /**
         * @brief Binds the vaos and draws the elements based on the RenderCommands available with the
         * `RenderPass`-
         *
         * @param renderPass
         */
        void doRender(helios::rendering::RenderPass& renderPass) const noexcept override;

        /**
         * @brief Ends the specified render pass. The current implementation does nothing.
         *
         * @param renderPass
         */
        void endRenderPass(helios::rendering::RenderPass& renderPass) const noexcept override;

        /**
         * @copydoc helios::rendering::RenderingDevice::setViewport()
         *
         * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml
         */
        void setViewport(const int x, const int y, const int width, const int height) const noexcept override;

        /**
         * @brief Clears the color buffer to preset values.
         *
         * @see clearColor
         */
        void clear() const  noexcept;

        /**
         * @brief Specifies the RGBa values clamped to [0, 1] when color
         * buffers are cleared.
         *
         * @param color
         *
         * @see clear
         */
        void setClearColor(const math::vec4f& color) const noexcept;

    };
}