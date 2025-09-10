module;

#include <string>

export module helios.platform.core:WindowConfig;


export namespace helios::platform {

    struct WindowConfig {
        int width{800};
        int height{600};
        std::string title{"helios - Window"};
        virtual ~WindowConfig() = default;
    };

}