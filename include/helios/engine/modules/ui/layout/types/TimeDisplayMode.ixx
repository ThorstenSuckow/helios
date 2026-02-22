/**
 * @file TimeDisplayMode.ixx
 * @brief Enum for selecting how time values are displayed.
 */
module;

#include <format>
#include <string>

export module helios.engine.modules.ui.layout.types.TimeDisplayMode;



export namespace helios::engine::modules::ui::layout::types {

    /**
     * @brief Controls whether a timer displays elapsed or remaining time.
     *
     * @see TimeFormatterComponent
     */
    enum class TimeDisplayMode  {

        /**
         * @brief Display the elapsed time since the timer started.
         */
        Elapsed,

        /**
         * @brief Display the remaining time until the timer expires.
         */
        Remaining

    };


}