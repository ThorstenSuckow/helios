/**
 * @file OpenGLDevice.ixx
 * @brief OpenGL-specific RenderingDevice implementation.
 */
module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <format>
#include <stdexcept>
#include <utility>

export module helios.ext.opengl.rendering.OpenGLDevice;

import helios.math.types;
import helios.rendering.RenderingDevice;
import helios.rendering.RenderQueue;
import helios.rendering.RenderPass;
import helios.rendering.model.config.PrimitiveType;
import helios.rendering.model.config.MeshConfig;
import helios.rendering.ClearFlags;
import helios.rendering.RenderTarget;
import helios.rendering.Viewport;

import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.shader.OpenGLShader;

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
        [[nodiscard]] GLenum toOpenGL(helios::rendering::model::config::PrimitiveType primitiveType) const noexcept {
            switch (primitiveType) {
                case helios::rendering::model::config::PrimitiveType::Points:
                    return GL_POINTS;
                case helios::rendering::model::config::PrimitiveType::Lines:
                    return GL_LINES;
                case helios::rendering::model::config::PrimitiveType::LineLoop:
                    return GL_LINE_LOOP;
                case helios::rendering::model::config::PrimitiveType::LineStrip:
                    return GL_LINE_STRIP;
                case helios::rendering::model::config::PrimitiveType::Triangles:
                    return GL_TRIANGLES;
                case helios::rendering::model::config::PrimitiveType::TriangleStrip:
                    return GL_TRIANGLE_STRIP;
                case helios::rendering::model::config::PrimitiveType::TriangleFan:
                    return GL_TRIANGLE_FAN;
                default:
                    logger_.warn("Failed to resolve primitive type, falling back to GL_TRIANGLES.");
                    return GL_TRIANGLES;
            }
        }


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
        void init() override {
            if (initialized_) {
                return;
            }
            const GLADloadfunc procAddressLoader = glfwGetProcAddress;
            const int gl_ver = gladLoadGL(procAddressLoader);

            if (gl_ver == 0) {
                logger_.error("Failed to load OpenGL");
                throw std::runtime_error("Failed to load OpenGL");
            }

            logger_.info(std::format("OpenGL {0}.{1} loaded", GLAD_VERSION_MAJOR(gl_ver), GLAD_VERSION_MINOR(gl_ver)));

            initialized_ = true;
        }

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
        void beginRenderPass(helios::rendering::RenderPass& renderPass) const noexcept override {
            const auto& viewport = renderPass.viewport();

            if (!viewport.renderTarget()) {
                logger_.warn("Viewport has no render target, skipping renderPass");
                return;
            }

            logger_.info("Begin RenderPass.");

            const helios::rendering::RenderTarget& renderTarget = *(viewport.renderTarget());
            const auto viewportBounds = viewport.bounds();
            const auto col = viewport.clearColor();
            glViewport(static_cast<int>(renderTarget.width() * viewportBounds[0]),
                       static_cast<int>(renderTarget.height() * viewportBounds[1]),
                       static_cast<int>(renderTarget.width() * viewportBounds[2]),
                       static_cast<int>(renderTarget.height() * viewportBounds[3]));


            glClearColor(col[0], col[1], col[2], col[3]);

            const int clearFlags = viewport.clearFlags();
            glClear(((clearFlags & std::to_underlying(helios::rendering::ClearFlags::Color)) ? GL_COLOR_BUFFER_BIT : 0) |
                    ((clearFlags & std::to_underlying(helios::rendering::ClearFlags::Depth)) ? GL_DEPTH_BUFFER_BIT : 0) |
                    ((clearFlags & std::to_underlying(helios::rendering::ClearFlags::Stencil)) ? GL_STENCIL_BUFFER_BIT
                                                                                               : 0));
        }

        /**
         * @brief Binds the VAOs and draws the elements based on the RenderCommands available with the RenderPass.
         *
         * @param renderPass The render pass containing the render queue to be processed.
         */
        void doRender(helios::rendering::RenderPass& renderPass) const noexcept override {
            const auto& renderQueue = renderPass.renderQueue();

            logger_.info(std::format("Rendering {0} item(s)...", renderQueue.count()));

            for (auto& rc: renderQueue.renderCommands()) {

                if (const auto renderPrototype_ptr = rc->renderPrototype().lock()) {
                    const auto& baseShader = renderPrototype_ptr->material().shader();
                    const auto& baseMesh = renderPrototype_ptr->mesh();

                    const auto* shader = dynamic_cast<const helios::ext::opengl::rendering::shader::OpenGLShader*>(&baseShader);
                    if (!shader) {
                        logger_.error("Failed to cast shader to OpenGLShader.");
                        continue;
                    }
                    logger_.info("activating shader...");
                    shader->use();
                    shader->applyUniformValues(renderPass.frameUniformValues());
                    shader->applyUniformValues(rc->objectUniformValues());
                    shader->applyUniformValues(rc->materialUniformValues());

                    const auto* mesh = dynamic_cast<const helios::ext::opengl::rendering::model::OpenGLMesh*>(&baseMesh);
                    if (!mesh) {
                        logger_.error("Failed to cast mesh to OpenGLMesh.");
                        continue;
                    }
                    const auto [primitiveType] = mesh->meshConfig();
                    logger_.info(std::format("Binding vao {0}", mesh->vao()));
                    glBindVertexArray(mesh->vao());
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glDrawElements(toOpenGL(primitiveType), mesh->indexCount(), GL_UNSIGNED_INT, nullptr);
                    glBindVertexArray(0);
                }
            }
        }

        /**
         * @brief Ends the specified render pass.
         *
         * The current implementation does nothing.
         *
         * @param renderPass The render pass to end.
         */
        void endRenderPass(helios::rendering::RenderPass& renderPass) const noexcept override {
        }



    };
} // namespace helios::ext::opengl::rendering
