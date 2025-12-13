/**
 * @file units.ixx
 * @brief Canonical length units and conversion helpers for helios to keep
 * spatial and temporal data consistent across modules.
 */
module;

#include <string>

export module helios.core.units;
import helios.util.log.Logger;
import helios.util.log.LogManager;

export namespace helios::core::units {

    /**
     * @brief Enumeration of length and time units supported by helios.
     */
    enum class Unit {
        /**
         * @brief Base length unit (meters) in helios.
         */
        Meter,

        /**
         * @brief Centimeter helper unit for authoring convenience.
         */
        Centimeter,

        /**
         * @brief Base time unit (seconds) in helios.
         */
        Seconds,

        /**
         * @brief Millisecond helper unit derived from seconds.
         */
        MilliSeconds
    };


    /**
     * @brief Default length unit used throughout helios (1 meter per helios unit).
     */
    constexpr auto HELIOS_SPATIAL_UNIT = Unit::Meter;

    /**
     * @brief Default time unit used for all timing APIs (seconds).
     */
    constexpr auto HELIOS_TEMPORAL_UNIT = Unit::Seconds;

    /**
     * @brief Ratio of centimeters to meters.
     */
    constexpr float CENTIMETERS = 0.01f;

    /**
     * @brief Ratio of meters to meters (identity).
     */
    constexpr float METERS = 1.0f;

    /**
     * @brief Ratio of milliseconds to seconds.
     */
    constexpr float MILLISECONDS = 0.001f;

    /**
     * @brief Ratio of seconds to seconds (identity).
     */
    constexpr float SECONDS = 1.0f;

    /**
     * @brief Converts centimeters to meters.
     *
     * @param cm Value expressed in centimeters.
     *
     * @return Converted value expressed in meters.
     */
    constexpr float fromCm(const float cm) noexcept {
        return  cm * CENTIMETERS;
    };

    /**
     * @brief Pass-through helper for meter values.
     *
     * @param m Value expressed in meters.
     *
     * @return Same value expressed in meters.
     */
    constexpr float fromM(const float m) noexcept {
        return  m * METERS;
    };

    /**
     * @brief Converts seconds to seconds (identity helper).
     */
    constexpr float fromS(const float s) noexcept {
        return s * SECONDS;
    }

    /**
     * @brief Converts milliseconds to seconds.
     */
    constexpr float fromMs(const float ms) noexcept {
        return ms * MILLISECONDS;
    }

    /**
     * @brief Converts a value from a given unit to helios units.
     *
     * This method takes a value in the specified unit and converts it to helios units,
     * e.g. spatial units to METER, temporal values to SECONDS.
     *
     * @param v Value to convert.
     * @param unit Unit of the value to convert.
     *
     * @return Value expressed in temporal or spatial helios units.
     */
    [[nodiscard]] constexpr float from(const float v, const Unit unit) noexcept {
        switch (unit) {
            case Unit::Meter:
                return fromM(v);
            case Unit::Centimeter:
                return fromCm(v);
            case Unit::Seconds:
                return fromS(v);
            case Unit::MilliSeconds:
                return fromMs(v);
        }

        std::unreachable();
    };

}
