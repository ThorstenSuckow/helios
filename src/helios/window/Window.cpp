module;

#include <string>

module helios.window.Window;

import helios.util.Guid;
import helios.window.WindowConfig;
import helios.math.types;
import helios.util.log.Logger;
import helios.util.log.LogManager;


namespace helios::window {


    Window::Window(const WindowConfig& cfg) :
        width_(cfg.width), height_(cfg.height), title_(cfg.title),
        viewport_(cfg.viewport),
        guid_(util::Guid::generate()){};


    [[nodiscard]] const util::Guid& Window::guid() const noexcept {
        return guid_;
    }


    [[nodiscard]] int Window::width() const noexcept {
        return width_;
    }


    [[nodiscard]] int Window::height() const noexcept {
        return height_;
    }


    [[nodiscard]] const math::vec4i& Window::viewport() const noexcept {
        return viewport_;
    }

    bool Window::operator==(const Window& win) const noexcept {
        return guid_ == win.guid();
    };

};
