module;

#include <string>

module helios.window.Window;

import helios.util.Guid;
import helios.window.WindowConfig;
import helios.math.types;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.rendering.RenderTarget;

using namespace helios::rendering;
namespace helios::window {


    Window::Window(
        std::unique_ptr<helios::rendering::RenderTarget> renderTarget,
        const WindowConfig& cfg
    ) :
    renderTarget_(std::move(renderTarget)),
    width_(cfg.width), height_(cfg.height), title_(cfg.title),
    guid_(util::Guid::generate()){};


    const util::Guid& Window::guid() const noexcept {
        return guid_;
    }


    int Window::width() const noexcept {
        return width_;
    }


    int Window::height() const noexcept {
        return height_;
    }


    RenderTarget& Window::renderTarget() const noexcept {
        return *renderTarget_;
    }


    bool Window::operator==(const Window& win) const noexcept {
        return guid_ == win.guid();
    };
};
