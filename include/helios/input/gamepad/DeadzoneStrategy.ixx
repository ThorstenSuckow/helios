/**
 * @file DeadzoneStrategy.ixx
 * @brief Abstract base class for gamepad axis deadzone normalization strategies.
 */
module;

export module helios.input.gamepad.DeadzoneStrategy;


export namespace helios::input::gamepad {

    /**
     * @brief Abstract strategy interface for normalizing gamepad stick input with deadzone handling.
     *
     * Gamepad analog sticks rarely return exactly (0, 0) when at rest due to hardware drift.
     * A deadzone defines a threshold below which input is ignored. Implementations of this
     * interface define how input values are normalized once they exceed the deadzone threshold.
     *
     * @see RadialDeadzoneStrategy for a circular deadzone implementation.
     */
    class DeadzoneStrategy {

    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~DeadzoneStrategy() = default;

        /**
         * @brief Default constructor.
         */
        DeadzoneStrategy() = default;

        /**
         * @brief Normalizes stick input values based on the configured deadzone.
         *
         * This method modifies the input coordinates in-place. Values within the deadzone
         * are typically zeroed out, while values outside are scaled to provide a smooth
         * response curve from the edge of the deadzone to the maximum input value.
         *
         * @param deadzone The deadzone threshold in the range [0.0, 1.0].
         * @param x Reference to the x-axis value, modified in-place.
         * @param y Reference to the y-axis value, modified in-place.
         */
        virtual void normalize(float deadzone, float& x, float& y) const noexcept = 0;


    };

}