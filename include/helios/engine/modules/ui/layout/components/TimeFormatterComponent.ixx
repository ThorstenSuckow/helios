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
         * @brief Label displayed instead of "00:00" when remaining time reaches zero.
         */
        std::string elapsedLabel_;

        /**
         * @brief True if an elapsed label has been configured.
         */
        bool hasElapsedLabel_ = false;

        /**
         * @brief True if the output should be empty when remaining time reaches zero.
         */
        bool hideWhenZero_ = false;

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

            const auto effectiveDisplayMode = duration == 0 ? TimeDisplayMode::Elapsed : displayMode_;


            switch (effectiveDisplayMode) {
                case TimeDisplayMode::Elapsed: {
                    const auto minutes = static_cast<unsigned int>(std::max(0.0f, elapsed)) / 60;
                    const auto seconds = static_cast<unsigned int>(std::max(0.0f, elapsed)) % 60;
                    return std::vformat(format_, std::make_format_args(minutes, seconds));
                }

                case TimeDisplayMode::Remaining: {

                    const auto minutes = static_cast<unsigned int>(std::max(0.0f, duration - elapsed)) / 60;
                    const auto seconds = static_cast<unsigned int>(std::max(0.0f, duration - elapsed)) % 60;

                    if (hideWhenZero_ && std::max(0.0f, duration - elapsed) <= 0.0f) {
                        return "";
                    }

                    if (hasElapsedLabel_ && std::max(1.0f, duration - elapsed) <= 1.0f) {
                        return elapsedLabel_;
                    }


                    return std::vformat(format_, std::make_format_args(minutes, seconds));

                }
            }

            std::unreachable();
        }

        /**
         * @brief Sets the label shown in place of "00:00" when remaining time reaches zero.
         *
         * In Remaining mode, once the remaining time drops to one second or below,
         * this label is returned by format() instead of the numeric output.
         * Implicitly enables the elapsed label.
         *
         * @param label The label string to display (e.g. "TIME UP").
         */
        void setElapsedLabel(std::string label) {
            elapsedLabel_ = std::move(label);
            hasElapsedLabel_ = true;
        }

        /**
         * @brief Returns the elapsed label.
         *
         * @return The configured elapsed label string.
         */
        std::string elapsedLabel() {
            return elapsedLabel_;
        }

        /**
         * @brief Returns whether an elapsed label is configured.
         *
         * @return True if an elapsed label has been set.
         */
        bool hasElapsedLabel() const {
            return hasElapsedLabel_;
        }

        /**
         * @brief Sets whether to hide the output when remaining time reaches zero.
         *
         * @param hideWhenZero True to return an empty string at zero remaining time.
         */
        void setHideWhenZero(const bool hideWhenZero) noexcept {
            hideWhenZero_ = hideWhenZero;
        }

        /**
         * @brief Returns whether the output is hidden at zero remaining time.
         *
         * @return True if hidden when zero.
         */
        bool hideWhenZero() const {
            return hideWhenZero_;
        }



    };


}