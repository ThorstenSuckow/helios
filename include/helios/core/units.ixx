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
    constexpr auto HELIOS_UNIT = Unit::Meter;

    /**
     * @brief Default time unit used for all timing APIs (seconds).
     */
    constexpr auto HELIOS_TIME_UNIT = Unit::Seconds;

    /**
     * @brief Ratio of centimeters to meters.
     */
    constexpr float CENTIMETERS = 0.01f;

    /**
     * @brief Ratio of meters to meters (identity).
     */
    constexpr float METERS = 1.0f;

    /**
     * @brief Converts centimeters to meters.
     *
     * @param cm Value expressed in centimeters.
     *
     * @return Converted value expressed in meters.
     */
    constexpr float cm(const float cm) noexcept {
        return  cm * 0.01f;
    };

    /**
     * @brief Pass-through helper for meter values.
     *
     * @param m Value expressed in meters.
     *
     * @return Same value expressed in meters.
     */
    constexpr float m(const float m) noexcept {
        return  m * 1.0f;
    };

    /**
     * @brief Converts seconds to seconds (identity helper).
     */
    constexpr float s(const float s) noexcept {
        return s * 1.0f;
    }

    /**
     * @brief Converts milliseconds to seconds.
     */
    constexpr float ms(const float ms) noexcept {
        return ms * 0.001f;
    }

    /**
     * @brief Converts a value into the requested unit.
     *
     * @param v Value to convert, expressed in helios units.
     * @param unit Target unit.
     *
     * @return Value expressed in the target unit.
     */
    constexpr float to(const float v, const Unit unit) noexcept {
        switch (unit) {
            case Unit::Meter:
                return m(v);
            case Unit::Centimeter:
                return cm(v);
            case Unit::Seconds:
                return s(v);
            case Unit::MilliSeconds:
                return ms(v);
        }

        return v;
    };

}
