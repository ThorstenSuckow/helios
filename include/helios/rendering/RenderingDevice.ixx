/**
 * @file RenderingDevice.ixx
 * @brief Low-level rendering device abstraction (OpenGL/Vulkan/etc.).
 */
module;

export module helios.rendering.RenderingDevice;

import helios.math.types;
import helios.util.log;
import helios.rendering.Renderable;
import helios.rendering.RenderQueue;
import helios.rendering.RenderPass;

import helios.rendering.text.TextRenderer;


#define HELIOS_LOG_SCOPE "helios::rendering::RenderingDevice"
export namespace helios::rendering {

    /**
     * @brief Abstract interface for a low-level rendering device.
     *
     * This pure virtual class provides the fundamental API for managing
     * the rendering pipeline and respective contracts for concrete
     * implementations using a specific rendering backend (e.g., OpenGL, Vulkan).
     *
     * ## Responsibilities
     *
     * - **Initialization:** Load graphics API function pointers and prepare resources.
     * - **Render Pass Management:** Begin, execute, and end render passes.
     * - **Geometry Rendering:** Process `RenderCommand` objects for mesh rendering.
     * - **Text Rendering:** Provide access to a `TextRenderer` for glyph-based text.
     *
     * ## Render Pass Lifecycle
     *
     * ```
     * beginRenderPass(pass)  →  doRender(pass)  →  endRenderPass(pass)
     * ```
     *
     * The `render()` convenience method executes all three steps in sequence.
     *
     * @see RenderPass
     * @see RenderQueue
     * @see TextRenderer
     */
    class RenderingDevice {

    protected:
        /**
         * @brief Reflects the initialization state of this device.
         */
        bool initialized_ = false;

        /**
         * @brief The logger used with this RenderingDevice class.
         *
         * Defaults to HELIOS_LOG_SCOPE.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE
        );

    public:
        virtual ~RenderingDevice() = default;

        /**
         * @brief Initializes this rendering device.
         *
         * Should be called when the graphics context was created and before
         * any rendering operations are performed.
         *
         * Implementing classes should load pointers to underlying GL functions
         * or provide similar API-specific setup.
         *
         * @throws std::runtime_error if initialization of the device fails.
         */
        virtual void init() = 0;

        /**
         * @brief Signals this device that the application is now ready to begin a new render pass.
         *
         * Implementing classes should prepare the rendering surface at this point and
         * clear specific buffers.
         *
         * This method should be called at the start of each frame before any drawing occurs.
         *
         * @param renderPass The RenderPass that is about to be processed by the rendering device.
         */
        virtual void beginRenderPass(helios::rendering::RenderPass& renderPass) const noexcept = 0;


        /**
         * @brief Advises this RenderingDevice to process the specified RenderPass.
         *
         * A `RenderPass` consists of `RenderCommand`s this RenderDevice executes, i.e.
         * binding meshes, materials and shaders as well as drawing geometry.
         *
         * @param renderPass A reference to the RenderPass that holds the `RenderCommand`s to be executed.
         */
        virtual void doRender(helios::rendering::RenderPass& renderPass) const noexcept = 0;


        /**
         * @brief Ends the specified render pass of this RenderingDevice.
         *
         * This method should be called when the renderPass was processed by the RenderingDevice,
         * before the window swaps the buffers.
         *
         * Implementing classes should take care of unbinding frame buffers and/or flushing
         * command buffers.
         *
         * @param renderPass The RenderPass that was processed by the rendering device.
         */
        virtual void endRenderPass(helios::rendering::RenderPass& renderPass) const noexcept = 0;

        /**
         * @brief Convenience method to subsequently call `beginRenderPass`, `doRender`, and `endRenderPass` (in this order) with the specified `RenderPass`.
         *
         * @param renderPass The RenderPass to process by this RenderingDevice.
         */
        void render(helios::rendering::RenderPass& renderPass) const {
            beginRenderPass(renderPass);
            doRender(renderPass);
            endRenderPass(renderPass);
        }

        /**
         * @brief Returns the text renderer associated with this device.
         *
         * Use this to register font families and access text rendering capabilities.
         *
         * @return Reference to the `TextRenderer` implementation.
         *
         * @see TextRenderer::addFontFamily()
         */
        [[nodiscard]] virtual helios::rendering::text::TextRenderer& textRenderer() const noexcept = 0;



        /**
         * @brief Returns the initialized state of this rendering device.
         *
         * This method is guaranteed to return `true` if the device was successfully
         * initialized, otherwise `false`.
         *
         * @return `true` if the device was successfully initialized, `false` otherwise.
         */
        [[nodiscard]] bool initialized() const noexcept{
            return initialized_;
        };
    };

}

