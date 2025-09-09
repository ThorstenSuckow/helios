module;

#include <cstdint>
#include <string>

export module helios.platform.core:WindowConfig;


export namespace helios::platform {

    struct WindowConfig {
        uint16_t width{800};
        uint16_t height{600};
        std::string title{"helios - Window"};
        virtual ~WindowConfig() = default;
    };

}