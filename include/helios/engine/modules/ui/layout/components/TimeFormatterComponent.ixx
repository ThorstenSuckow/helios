/**
 * @file TimeFormatterComponent.ixx
 * @brief Component that formats time values for UI display.
 */
module;

#include <format>
#include <string>

export module helios.engine.modules.ui.layout.components.TimeFormatterComponent;

import helios.engine.modules.ui.layout.types.TimeDisplayMode;

using namespace helios::engine::modules::ui::layout::types;

export namespace helios::engine::modules::ui::layout::components {

    /**
     * @brief Component that formats elapsed or remaining time into a display string.
     *
     * Holds a format string compatible with std::vformat and a TimeDisplayMode.
     * The format string receives two integer arguments: minutes and seconds.
     *
     * @see TimeDisplayMode
     * @see GameTimer2UiTextUpdateSystem
     */
    class TimeFormatterComponent  {

        /**
         * @brief The format string used by std::vformat (expects minutes, seconds).
         */
        std::string format_;

        /**
         * @brief The active display mode (elapsed or remaining).
         *
         * Declared mutable because format() may fall back to Elapsed when
         * duration is zero.
         */
        mutable TimeDisplayMode displayMode_ = TimeDisplayMode::Elapsed;


    public:

        /**
         * @brief Sets the format string and display mode.
         *
         * @param format A std::vformat-compatible string (e.g. "{:02d}:{:02d}").
         * @param displayMode The display mode to use. Defaults to Elapsed.
         */
        void setFormat(std::string format, const TimeDisplayMode displayMode = TimeDisplayMode::Elapsed) {
            format_ = std::move(format);
            displayMode_ = displayMode;
        }

        /**
         * @brief Formats the given time values into a display string.
         *
         * If duration is zero, the display mode falls back to Elapsed.
         *
         * @param elapsed Elapsed time in seconds.
         * @param duration Total duration in seconds. Zero disables Remaining mode.
         *
         * @return The formatted time string.
         */
        std::string format(const float elapsed, const float duration = 0) const {

            if (duration == 0) {
                displayMode_ = TimeDisplayMode::Elapsed;
            }

            switch (displayMode_) {
                case TimeDisplayMode::Elapsed: {
                    const auto minutes = static_cast<unsigned int>(std::max(0.0f, elapsed)) / 60;
                    const auto seconds = static_cast<unsigned int>(std::max(0.0f, elapsed)) % 60;
                    return std::vformat(format_, std::make_format_args(minutes, seconds));
                }

                case TimeDisplayMode::Remaining: {
                    const auto minutes = static_cast<unsigned int>(std::max(0.0f, duration - elapsed)) / 60;
                    const auto seconds = static_cast<unsigned int>(std::max(0.0f, duration - elapsed)) % 60;
                    return std::vformat(format_, std::make_format_args(minutes, seconds));
                }
            }

            std::unreachable();
        }
    };


}