module;

export module helios.rendering.core:RenderingDevice;

export namespace helios::rendering::core {

    class RenderingDevice {
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
    };

}