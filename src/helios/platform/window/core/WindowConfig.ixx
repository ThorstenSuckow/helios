module;

#include <string>

export module helios.platform.window.core:WindowConfig;

import helios.math.types;

export namespace helios::platform::window::core {

    struct WindowConfig {
        int width{800};
        int height{600};
        std::string title{"helios - Window"};
        math::vec4 viewport{0,0, 800, 600};
        virtual ~WindowConfig() = default;
    };

}