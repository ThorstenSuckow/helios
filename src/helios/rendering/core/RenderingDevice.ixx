module;

export module helios.rendering.core.RenderingDevice;

import helios.math.types;

export namespace helios::rendering::core {


    class RenderingDevice {

    protected:
        bool initialized_ = false;

    public:
        virtual ~RenderingDevice() = default;

        /**
         * Initializes this rendering device.
         * Implementing APIs should load pointers to underlying GLs at
         * this point.
         *
         * @return
         */
        virtual bool init() noexcept = 0;

        /**
         * Signals this device that the application is now ready to begin rendering.
         * Implementing APIs should take means to reset the rendering surface,
         * clear color buffers and so on.
         *
         */
        virtual void beginRenderPass() const noexcept = 0;

        /**
         * Sets the viewport for this RenderingDevice.
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @return
         */
        virtual void setViewport(int x, int y, int width, int height) const noexcept = 0;

        /**
         * Returns the initialized state of this rendering device.
         * @return
         */
        [[nodiscard]] bool initialized() const noexcept{
            return initialized_;
        };
    };

}