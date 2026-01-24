/**
 * @file DeadzoneStrategy.ixx
 * @brief Abstract base class for gamepad axis deadzone normalization strategies.
 */
module;

#include <algorithm>

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

    protected:

        /**
         * @brief Threshold used to filter out minor noise in gamepad stick input.
         *
         * This value represents the minimum magnitude of stick input required
         * to be treated as valid. Input signals with a magnitude below this
         * threshold are considered noise, helping to mitigate the effect of
         * hardware drift or minor stick movements. Adjusting this parameter
         * can improve input precision and reduce unintended drift behavior.
         */
        float stickNoiseThreshold_ = DEFAULT_STICK_NOISE_THRESHOLD;

    public:

        /**
         * @brief Default threshold used to filter out minor noise in gamepad stick input.
         */
        static constexpr float DEFAULT_STICK_NOISE_THRESHOLD = 0.002f;

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~DeadzoneStrategy() = default;

        /**
         * @brief Default constructor.
         */
        DeadzoneStrategy() = default;

        /**
         * @brief Interface for applying deadzone handling to gamepad stick input.
         *
         * Defines a method for processing and normalizing analog stick input while accounting
         * for a deadzone. A deadzone eliminates unintended small input values caused
         * by hardware imperfections. Implementations of this interface provide specific
         * strategies for handling the deadzone and input normalization.
         *
         * @see AxisDeadzoneStrategy for an axis-aligned deadzone implementation.
         */
        DeadzoneStrategy(float stickNoiseThreshold) : stickNoiseThreshold_(stickNoiseThreshold) {}

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


        /**
         * @brief Retrieves the current noise threshold for stick input.
         *
         * The noise threshold determines the minimum magnitude of input
         * required to be processed as valid. Values below this threshold are
         * considered noise and ignored to prevent unintentional drift detection.
         *
         * @return The configured noise threshold value.
         */
        [[nodiscard]] float stickNoiseThreshold() const noexcept {
            return stickNoiseThreshold_;
        }

        /**
         * @brief Sets the noise threshold for stick inputs to handle minor hardware drift.
         *
         * The noise threshold defines the minimum magnitude for stick input to be considered valid.
         * Inputs with a magnitude below this threshold are ignored to prevent unintentional movement
         * caused by hardware noise or drift.
         *
         * @param threshold The new stick noise threshold, typically in the range [0.0, 1.0].
         */
        void setStickNoiseThreshold(float threshold) noexcept {
            stickNoiseThreshold_ = std::clamp(threshold, 0.0f, 0.9f);
        }
    };

}