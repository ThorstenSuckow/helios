module;

#include <stdexcept>

module helios.window.Window;

import helios.util.Guid;
import helios.window.WindowConfig;
import helios.math.types;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.rendering.RenderTarget;
import helios.rendering.Viewport;

using namespace helios::rendering;

namespace helios::window {


    Window::Window(
        std::unique_ptr<helios::rendering::RenderTarget> renderTarget,
        const WindowConfig& cfg
    ) :
    renderTarget_(std::move(renderTarget)),
    width_(cfg.width),
    height_(cfg.height),
    title_(cfg.title),
    aspectRatioNumer_(cfg.aspectRatioNumer),
    aspectRatioDenom_(cfg.aspectRatioDenom),
    guid_(util::Guid::generate()) {

        if (!renderTarget_) {
            throw std::invalid_argument("Window received a nullptr renderTarget");
        }
    }


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

    std::shared_ptr<Viewport> Window::addViewport(std::shared_ptr<Viewport> viewport) const {
        return renderTarget_->addViewport(std::move(viewport));
    }


    bool Window::operator==(const Window& win) const noexcept {
        return guid_ == win.guid();
    };
};
