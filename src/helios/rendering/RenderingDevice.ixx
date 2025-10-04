module;

export module helios.rendering.RenderingDevice;

import helios.math.types;
import helios.util.log;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderingDevice"
export namespace helios::rendering {


    /**
     * Abstract interface for low level rendering device.
     * This class provides fundamental API for managing the rendering pipeline.
     * Concrete implementations provide API specific logic.
     */
    class RenderingDevice {

    protected:
        /**
         * Reflects the initialization state of this viewport.
         */
        bool initialized_ = false;

        /**
         * The logger used with this Material instance.
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
         * Initializes this rendering device.
         * Should be called when the graphics context was created and before
         * any rendering operations are performed.
         * Implementing APIs should load pointers to underlying GL functions
         * por provide similar API-specific setup.
         *
         * @throws if initializing this device failed.
         */
        virtual void init() = 0;


        /**
         * Signals this device that the application is now ready to begin a new render pass.
         * Implementing APIs should consider to prepare the rendering surface at this point and
         * clear specific buffers.
         * This method should be called at the start of each frame before any drawing occurs.
         */
        virtual void beginRenderPass() const noexcept = 0;


        /**
         * Sets the viewport for this RenderingDevice.
         * The viewport defines the rectangular area where the final image is drawn.
         *
         * @param x x-coordinate of the lower left corner of the viewport
         * @param y y-coordinate of the lower left corner of the viewport
         * @param width width of the viewport, in pixels
         * @param height height of the viewport, in pixels
         * @return
         */
        virtual void setViewport(int x, int y, int width, int height) const noexcept = 0;


        /**
         * Returns the initialized state of this rendering device.
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