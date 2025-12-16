/**
 * @file RadialDeadzoneStrategy.ixx
 * @brief Radial (circular) deadzone strategy for gamepad analog stick input.
 */
module;

export module helios.input.gamepad.RadialDeadzoneStrategy;

import helios.input.gamepad.DeadzoneStrategy;


export namespace helios::input::gamepad {

    /**
     * @brief Implements a radial (circular) deadzone strategy for analog stick normalization.
     *
     * This strategy treats the deadzone as a circle centered at the origin. Input vectors
     * with a magnitude less than or equal to the deadzone threshold are zeroed out completely.
     * Input vectors exceeding the deadzone are rescaled so that the effective input range
     * starts at the edge of the deadzone circle, providing a smooth and consistent response.
     *
     * The radial approach is generally preferred over axial deadzones because it eliminates
     * diagonal movement artifacts and provides uniform sensitivity in all directions.
     *
     * **Algorithm:**
     * 1. Calculate the magnitude (length) of the input vector.
     * 2. If magnitude <= deadzone, set both axes to zero.
     * 3. If magnitude > 1.0, normalize the vector to unit length.
     * 4. Rescale the magnitude from [deadzone, 1.0] to [0.0, 1.0].
     * 5. Apply the rescaled magnitude to the normalized direction vector.
     *
     * @see DeadzoneStrategy for the abstract interface.
     */
    class RadialDeadzoneStrategy : public DeadzoneStrategy {

    public:

        /**
         * @copydoc helios::input::gamepad::DeadzoneStrategy::DeadzoneStrategy()
         */
        RadialDeadzoneStrategy() : DeadzoneStrategy() {}

        /**
         * @copydoc helios::input::gamepad::DeadzoneStrategy::DeadzoneStrategy(float stickNoiseThreshold)
         */
        RadialDeadzoneStrategy(float stickNoiseThreshold) : DeadzoneStrategy(stickNoiseThreshold) {}

        /**
         * @brief Applies radial deadzone normalization to stick input.
         *
         * Modifies the input coordinates in-place. Values within the circular deadzone
         * are zeroed, while values outside are rescaled for a smooth response curve.
         *
         * @param deadzone The deadzone radius threshold in the range [0.0, 1.0].
         * @param x Reference to the x-axis value, modified in-place.
         * @param y Reference to the y-axis value, modified in-place.
         */
        void normalize(float deadzone, float& x, float& y) const noexcept override;


    };


}