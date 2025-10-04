module;

#include <string>

export module helios.window.WindowConfig;

import helios.math.types;

export namespace helios::window {

    struct WindowConfig {
        int width{800};
        int height{600};
        std::string title{"helios - Window"};
        math::vec4i viewport{0,0, 800, 600};
        virtual ~WindowConfig() = default;
    };

}