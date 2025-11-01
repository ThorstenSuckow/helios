module;

export module helios.rendering.RenderingDevice;

import helios.math.types;
import helios.util.log;
import helios.rendering.Renderable;
import helios.rendering.RenderQueue;
import helios.rendering.RenderPass;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderingDevice"
export namespace helios::rendering {

    /**
     * @brief Abstract interface for low level rendering device.
     *
     * This pure virtual class provides the fundamental API for managing
     * the rendering pipeline and respective contracts for concrete
     * implementations using a specific rendering backend (e.g. OpenGL,
     * Vulkan...).
     *
     * Implementations of `RenderingDevice` are responsible for managing
     * render passes, and configuring the viewport.
     */
    class RenderingDevice {

    protected:
        /**
         * @brief Reflects the initialization state of this viewport.
         */
        bool initialized_ = false;

        /**
         * @brief The logger used with this Material instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:
        virtual ~RenderingDevice() = default;

        /**
         * @brief Initializes this rendering device.
         * Should be called when the graphics context was created and before
         * any rendering operations are performed.
         * Implementing APIs should load pointers to underlying GL functions
         * or provide similar API-specific setup.
         *
         * @throws if initializing this device failed.
         */
        virtual void init() = 0;

        /**
         * @brief Signals this device that the application is now ready to begin a new render pass.
         * Implementing APIs should consider to prepare the rendering surface at this point and
         * clear specific buffers.
         * This method should be called at the start of each frame before any drawing occurs.
         *
         * @param renderPass The RenderPass that is about to be processed by the rendering device.
         */
        virtual void beginRenderPass(helios::rendering::RenderPass& renderPass) const noexcept = 0;


        /**
         * @brief Advises this RenderingDevice to process the specified RenderPass.
         * A `RenderPass` consists of `RenderCommand`s this RenderDevice executes, i.e.
         * binding meshes, materials and shaders as well as drawing geometry.
         *
         * @param renderPass A ref to the RenderQueue that holds the `RenderCommand`s
         * to be executed.
         */
        virtual void doRender(helios::rendering::RenderPass& renderPass) const noexcept = 0;


        /**
         * @brief Ends the specified render pass of this RenderingDevice.
         * This method should be called when the renderPass was processed by the RenderingDevice,
         * before the window swaps the buffers.
         * Implementing classes should take care of unbinding frame buffers and / or flushing
         * command bufefrs.
         *
        * @param renderPass The RenderPass that was processed by the rendering device.
         */
        virtual void endRenderPass(helios::rendering::RenderPass& renderPass) const noexcept = 0;

        /**
         * @brief Convenient method to subsequently call `beginRenderPass`,
         * `doRender`, `endRenderPass` (in this order) with the specified `RenderPass`.
         *
         * @param renderPass The RenderPass to process by this RenderingDevice.
         */
        void render(helios::rendering::RenderPass& renderPass) {
            beginRenderPass(renderPass);
            doRender(renderPass);
            endRenderPass(renderPass);
        }

        /**
         * @brief Sets the viewport for this RenderingDevice.
         * The viewport defines the rectangular area where the final image is drawn.
         *
         * @param x x-coordinate of the lower left corner of the viewport
         * @param y y-coordinate of the lower left corner of the viewport
         * @param width width of the viewport, in pixels
         * @param height height of the viewport, in pixels
         * @return
         */
        virtual void setViewport(const int x, const int y, const int width, const int height) const noexcept = 0;


        /**
         * @brief Returns the initialized state of this rendering device.
         * This method is guaranteed to return true if the device was successfully
         * initialized, otherwise false.
         *
         * @return true if the device was successfully initialized, otherwise false.
         */
        [[nodiscard]] bool initialized() const noexcept{
            return initialized_;
        };
    };

}