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
     * The struct provides common parameters, such as width, height and title.
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
         * @brief Initial title displayed with the window, e.g. the title bar.
         */
        std::string title{"helios - Window"};

    };

}