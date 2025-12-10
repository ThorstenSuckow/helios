/**
 * @file WindowConfig.ixx
 * @brief Window creation and initialization configuration.
 */
module;

#include <string>

export module helios.window.WindowConfig;

import helios.math.types;

export namespace helios::window {

    /**
     * @brief Configuration structure for creating and initializing a Window.
     *
     * Provides common parameters such as dimensions, aspect ratio, and title.
     */
    struct WindowConfig {

        virtual ~WindowConfig() = default;

        /**
         * @brief Initial width of the Window.
         */
        int width{800};


        /**
         * @brief Initial height of the Window.
         */
        int height{800};

        /**
         * @brief Numerator of the aspect ratio.
         *
         * This value represents the numerator part of the aspect ratio
         * used for the window. It is paired with `aspectRatioDenom` to
         * define the aspect ratio as a fraction (numerator/denominator).
         * A value of 0 indicates that no specific aspect ratio is set.
         */
        int aspectRatioNumer{0};

        /**
         * @brief Denominator of the aspect ratio.
         *
         * This value represents the denominator part of the aspect ratio
         * used for the window. It is paired with `aspectRatioNumer` to
         * define the aspect ratio as a fraction (numerator/denominator).
         * A value of 0 indicates that no specific aspect ratio is set.
         */
        int aspectRatioDenom{0};


        /**
         * @brief Initial title displayed with the window, e.g. the title bar.
         */
        std::string title{"helios - Window"};

    };

}