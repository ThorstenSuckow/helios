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
        virtual RenderingDevice& init() = 0;

        /**
         * Enters rendering.
         *
         */
        virtual RenderingDevice& beginRenderPass() {
            return *this;
        };

        /**
         * Sets the viewport for this RenderingDevice.
         *
         * @param x
         * @param y
         * @param width
         * @param height
         * @return
         */
        virtual RenderingDevice& setViewport(int x, int y, int width, int height) = 0;

        /**
         * Returns the initialized state of this rendering device.
         * @return
         */
        [[nodiscard]] bool initialized() const noexcept{
            return initialized_;
        };
    };

}