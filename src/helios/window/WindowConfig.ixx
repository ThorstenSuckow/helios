module;

#include <string>

export module helios.window.WindowConfig;

import helios.math.types;

export namespace helios::window {

    /**
     * Configuration structure for creating and initializing a Window.
     *
     * The struct provides configuration of common parameters, such as
     * width, height, the title and the viewport.
     */
    struct WindowConfig {

        virtual ~WindowConfig() = default;

        /**
         * Initial width of the Window.
         */
        int width{800};


        /**
         * Initial width of the Window.
         */
        int height{600};


        /**
         * Initial title displayed with the window, e.g. the title bar.
         */
        std::string title{"helios - Window"};


        /**
         * Initial viewport configuration (x, y, width, height).
         */
        math::vec4i viewport{0,0, 800, 600};

    };

}