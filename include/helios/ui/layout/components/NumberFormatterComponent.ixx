/**
 * @file NumberFormatterComponent.ixx
 * @brief Component that formats numeric values for UI display.
 */
module;

#include <format>
#include <string>

export module helios.ui.layout.components.NumberFormatterComponent;


export namespace helios::ui::layout::components {

    /**
     * @brief Component that formats arithmetic values into a display string.
     *
     * Holds a std::vformat-compatible format string that accepts a single
     * arithmetic argument. Used by binding systems to convert numeric game
     * state (e.g. scores, health) into text for UiTextComponent.
     *
     * @see Score2UiTextUpdateSystem
     * @see MaxScore2UiTextUpdateSystem
     */
    template<typename THandle>
    class NumberFormatterComponent  {

        /**
         * @brief The format string used by std::vformat.
         */
        std::string format_ = "{:0.f}";



    public:

        /**
         * @brief Sets the format string.
         *
         * @param format A std::vformat-compatible string (e.g. "{:06.0f}").
         */
        void setFormat(std::string format) {
            format_ = std::move(format);
        }

        /**
         * @brief Formats the given arithmetic value into a display string.
         *
         * @tparam T An arithmetic type (int, float, double, etc.).
         *
         * @param value The value to format.
         *
         * @return The formatted string.
         */
        template<typename T>
        requires std::is_arithmetic_v<T>
        std::string format(T value) const {
            return std::vformat(format_, std::make_format_args(value));
        }

    };


}