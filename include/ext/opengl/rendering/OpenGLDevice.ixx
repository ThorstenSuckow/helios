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
#include <cassert>

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

import helios.rendering.text.TextRenderer;

import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.ext.opengl.rendering.OpenGLGlyphTextRenderer;

export namespace helios::ext::opengl::rendering {

    /**
     * @brief OpenGL implementation of `RenderingDevice`.
     *
     * `OpenGLDevice` is the concrete OpenGL backend for the helios rendering system.
     * It handles all OpenGL-specific operations including mesh rendering, shader management,
     * and text rendering via `OpenGLGlyphTextRenderer`.
     *
     * ## Responsibilities
     *
     * - **Initialization:** Load OpenGL function pointers via GLAD.
     * - **Render Pass Execution:** Configure viewport, clear buffers, and process render commands.
     * - **Mesh Rendering:** Bind VAOs and issue draw calls for geometry.
     * - **Text Rendering:** Delegate text commands to `OpenGLGlyphTextRenderer`.
     *
     * ## Usage
     *
     * ```cpp
     * auto textRenderer = std::make_unique<OpenGLGlyphTextRenderer>();
     * auto device = std::make_unique<OpenGLDevice>(std::move(textRenderer));
     *
     * // After creating OpenGL context
     * device->init();
     *
     * // Load fonts via the text renderer
     * device->textRenderer().addFontFamily(FontId{1}, "fonts/arial.ttf");
     * ```
     *
     * @see RenderingDevice
     * @see OpenGLGlyphTextRenderer
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

        /**
         * @brief Text renderer for FreeType-based glyph rendering.
         */
        std::unique_ptr<helios::ext::opengl::rendering::OpenGLGlyphTextRenderer> textRenderer_;

        mutable const helios::ext::opengl::rendering::shader::OpenGLShader* lastShader_ = nullptr;
        mutable unsigned int lastVao_ = 0;


    public:
        ~OpenGLDevice() override = default;

        /**
         * @brief Constructs an OpenGLDevice with the given text renderer.
         *
         * @param textRenderer The text renderer to use for glyph-based text rendering.
         *                     Ownership is transferred to this device.
         */
        explicit OpenGLDevice(
            std::unique_ptr<helios::ext::opengl::rendering::OpenGLGlyphTextRenderer> textRenderer
        ) :
        textRenderer_(std::move(textRenderer)) {}

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

            textRenderer_->init();

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

            assert(viewport.renderTarget() && "Unexpected missing render target for viewport");

            const helios::rendering::RenderTarget& renderTarget = *(viewport.renderTarget());
            const auto viewportBounds = viewport.bounds();
            const auto col = viewport.clearColor();
            glViewport(static_cast<int>(renderTarget.width() * viewportBounds[0]),
                       static_cast<int>(renderTarget.height() * viewportBounds[1]),
                       static_cast<int>(renderTarget.width() * viewportBounds[2]),
                       static_cast<int>(renderTarget.height() * viewportBounds[3]));


            // this is equally important for the GlpyhTextRenderer
            // enable blending since the font's fragment shader uses the alpha channel
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


            glClearColor(col[0], col[1], col[2], col[3]);

            const int clearFlags = viewport.clearFlags();
            glClear(((clearFlags & std::to_underlying(helios::rendering::ClearFlags::Color)) ? GL_COLOR_BUFFER_BIT : 0) |
                    ((clearFlags & std::to_underlying(helios::rendering::ClearFlags::Depth)) ? GL_DEPTH_BUFFER_BIT : 0) |
                    ((clearFlags & std::to_underlying(helios::rendering::ClearFlags::Stencil)) ? GL_STENCIL_BUFFER_BIT
                                                                                               : 0));
        }

        /**
         * @brief Processes render commands and draws geometry and text.
         *
         * Iterates through all render commands in the render queue, binds the appropriate
         * shaders and VAOs, applies uniform values, and issues draw calls. After processing
         * geometry, delegates any text render commands to `renderTextCommands()`.
         *
         * @param renderPass The render pass containing the render queue to be processed.
         *
         * @see renderTextCommands()
         */
        void doRender(helios::rendering::RenderPass& renderPass) const noexcept override {
            const auto& renderQueue = renderPass.renderQueue();



            for (auto& rc: renderQueue.renderCommands()) {

                if (const auto renderPrototype_ptr = rc->renderPrototype().lock()) {
                    const auto& baseShader = renderPrototype_ptr->material().shader();
                    const auto& baseMesh = renderPrototype_ptr->mesh();

                    const auto* shader = static_cast<const helios::ext::opengl::rendering::shader::OpenGLShader*>(&baseShader);
                    assert(shader && "Unexpected failure when casting to OpenGLShader.");

                    if (shader != lastShader_) {
                        shader->use();
                        lastShader_ = shader;
                    }

                    shader->applyUniformValues(renderPass.frameUniformValues());
                    shader->applyUniformValues(rc->objectUniformValues());
                    shader->applyUniformValues(rc->materialUniformValues());

                    const auto* mesh = static_cast<const helios::ext::opengl::rendering::model::OpenGLMesh*>(&baseMesh);
                    assert(mesh && "Unexpected failure when casting to OpenGLMesh.");

                    const auto [primitiveType] = mesh->meshConfig();

                    if (mesh->vao() != lastVao_) {
                        glBindVertexArray(mesh->vao());
                        lastVao_ = mesh->vao();
                    }

                    glDrawElements(toOpenGL(primitiveType), mesh->indexCount(), GL_UNSIGNED_INT, nullptr);
                }
            }

            if (renderQueue.textRenderCommandsSize() > 0) {
                renderTextCommands(renderPass);
            }
        }

        /**
         * @brief Renders all text commands in the render pass.
         *
         * Iterates through the text render commands in the render queue and delegates
         * each command to the `OpenGLGlyphTextRenderer` for rendering.
         *
         * @param renderPass The render pass containing text render commands.
         */
        void renderTextCommands(const helios::rendering::RenderPass& renderPass) const noexcept {
            const auto& renderQueue = renderPass.renderQueue();

            for (auto& rc: renderQueue.textRenderCommands()) {

                textRenderer_->render(rc, renderPass.frameUniformValues());//shader, text, screenPosition, scale);
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
            glBindVertexArray(0);
        }

        /**
         * @brief Returns a reference to the text renderer.
         *
         * Use this to register font families before rendering text.
         *
         * @return Reference to the `TextRenderer` interface.
         *
         * @see OpenGLGlyphTextRenderer::addFontFamily()
         */
        [[nodiscard]] helios::rendering::text::TextRenderer& textRenderer() const noexcept {
            return *textRenderer_;
        }


    };
} // namespace helios::ext::opengl::rendering
