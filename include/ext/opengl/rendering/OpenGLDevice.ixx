/**
 * @file OpenGLDevice.ixx
 * @brief OpenGL-specific RenderingDevice implementation.
 */
module;

#include <glad/gl.h>

export module helios.ext.opengl.rendering.OpenGLDevice;

import helios.math.types;
import helios.rendering.RenderingDevice;
import helios.rendering.RenderQueue;
import helios.rendering.RenderPass;
import helios.rendering.model.config.PrimitiveType;

export namespace helios::ext::opengl::rendering {

    /**
     * @brief OpenGL RenderingDevice representative.
     */
    class OpenGLDevice : public  helios::rendering::RenderingDevice {

        /**
         * @brief Translates helios abstract PrimitiveType enum to its corresponding OpenGL GLenum value.
         *
         * This is a utility function used exclusively by the OpenGLDevice to interpret the
         * API-agnostic primitive type requested by the helios configuration.
         *
         * @param primitiveType The API-agnostic PrimitiveType value.
         *
         * @return The corresponding OpenGL primitive type as a GLenum. If mapping does not
         * succeed, the function falls back to GL_TRIANGLES.
         */
        [[nodiscard]] GLenum toOpenGL(helios::rendering::model::config::PrimitiveType primitiveType) const noexcept;


    public:
        ~OpenGLDevice() override = default;

        /**
         * @brief Initializes the OpenGL device to access modern OpenGL.
         *
         * This method must be called **after** the helios application's `setCurrent()` was called
         * for creating a current context. This OpenGL device will then load from this context.
         *
         * This method uses the `gladLoadGL` method in conjunction with `glfwGetProcAddress`,
         * which returns the address of the specific OpenGL core.
         *
         * @see https://www.glfw.org/docs/latest/group__context.html#ga35f1837e6f666781842483937612f163
         *
         * @todo Provide abstraction for glfwGetProcAddress.
         */
        void init() override;

        /**
         * @brief Begins a new render pass.
         *
         * This implementation makes sure that the rendering surface is cleared
         * with the current clear color, which can be configured via `clearColor()`.
         *
         * @param renderPass The render pass to begin.
         *
         * @see clear()
         * @see clearColor()
         */
        void beginRenderPass(helios::rendering::RenderPass& renderPass) const noexcept override;

        /**
         * @brief Binds the VAOs and draws the elements based on the RenderCommands available with the RenderPass.
         *
         * @param renderPass The render pass containing the render queue to be processed.
         */
        void doRender(helios::rendering::RenderPass& renderPass) const noexcept override;

        /**
         * @brief Ends the specified render pass.
         *
         * The current implementation does nothing.
         *
         * @param renderPass The render pass to end.
         */
        void endRenderPass(helios::rendering::RenderPass& renderPass) const noexcept override;



    };
}

