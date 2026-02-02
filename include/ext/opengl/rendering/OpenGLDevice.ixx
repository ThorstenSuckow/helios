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
import helios.rendering.mesh.PrimitiveType;
import helios.rendering.mesh.MeshConfig;
import helios.rendering.ClearFlags;
import helios.rendering.RenderTarget;
import helios.rendering.Viewport;

import helios.rendering.text.TextRenderer;
import helios.rendering.text.FontResourceProvider;

import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.ext.opengl.rendering.OpenGLGlyphTextRenderer;
import helios.ext.opengl.rendering.OpenGLMeshRenderer;

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
         * @brief Text renderer for FreeType-based glyph rendering.
         */
        std::unique_ptr<helios::ext::opengl::rendering::OpenGLGlyphTextRenderer> textRenderer_;

        /**
         * @brief Mesh renderer for geometry rendering.
         */
        std::unique_ptr<helios::ext::opengl::rendering::OpenGLMeshRenderer> meshRenderer_;

        /**
         * @brief Font resource provider for loading fonts and retrieving glyph data.
         */
        std::unique_ptr<helios::rendering::text::FontResourceProvider> fontResourceProvider_;


    public:
        ~OpenGLDevice() override = default;

        /**
         * @brief Constructs an OpenGLDevice with the given renderers and font provider.
         *
         * @param meshRenderer The mesh renderer for geometry rendering.
         * @param textRenderer The text renderer for glyph-based text rendering.
         * @param fontResourceProvider The font resource provider for loading fonts.
         */
        explicit OpenGLDevice(
            std::unique_ptr<helios::ext::opengl::rendering::OpenGLMeshRenderer> meshRenderer,
            std::unique_ptr<helios::ext::opengl::rendering::OpenGLGlyphTextRenderer> textRenderer,
            std::unique_ptr<helios::rendering::text::FontResourceProvider> fontResourceProvider
        ) :
        meshRenderer_(std::move(meshRenderer)),
        textRenderer_(std::move(textRenderer)),
        fontResourceProvider_(std::move(fontResourceProvider)){}

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
            meshRenderer_->beginRenderPass(renderPass);

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

            if (renderQueue.meshRenderCommandsSize() > 0) {
                renderMeshCommands(renderPass);
            }

            if (renderQueue.textRenderCommandsSize() > 0) {
                renderTextCommands(renderPass);
            }
        }

        /**
         * @brief Renders all mesh commands in the render pass.
         *
         * Iterates through the mesh render commands in the render queue and delegates
         * each command to the `OpenGLMeshRenderer` for rendering.
         *
         * @param renderPass The render pass containing mesh render commands.
         */
        void renderMeshCommands(const helios::rendering::RenderPass& renderPass) const noexcept {

            const auto& renderQueue = renderPass.renderQueue();

            for (auto& rc: renderQueue.meshRenderCommands()) {
                meshRenderer_->render(rc, renderPass.frameUniformValues());
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
                textRenderer_->render(rc, renderPass.frameUniformValues());
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

        /**
         * @brief Returns a reference to the font resource provider.
         *
         * Use this to load fonts before creating `TextRenderPrototype` instances.
         *
         * @return Reference to the `FontResourceProvider` implementation.
         */
        [[nodiscard]] helios::rendering::text::FontResourceProvider& fontResourceProvider() const noexcept override {
            return *fontResourceProvider_;
        }


    };
} // namespace helios::ext::opengl::rendering
