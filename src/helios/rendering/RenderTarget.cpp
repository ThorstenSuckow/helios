module;

#include <memory>
#include <stdexcept>
#include <vector>

module helios.rendering.RenderTarget;

using namespace helios::rendering;


namespace helios::rendering {
    RenderTarget::RenderTarget(
        std::shared_ptr<Viewport> viewport,
        const unsigned int width,
        const unsigned int height)
        : width_(width),
          height_(height) {
        if (!viewport) {
            throw std::invalid_argument("Constructor received a null shared pointer");
        }

        addViewport(std::move(viewport));
    }

    const Viewport& RenderTarget::addViewport(
        std::shared_ptr<Viewport> viewport) {

        if (!viewport) {
            throw std::invalid_argument("addViewport() received a null shared pointer");
        }

        if (viewport->renderTarget()) {
            throw std::invalid_argument("Viewport already belongs to a RenderTarget.");
        }

        viewport->setRenderTarget(&*this, viewportKey_);
        viewports_.emplace_back(std::move(viewport));

        return *(viewports_.back());
    }

    void RenderTarget::setSize(const int width, const int height) noexcept {
        width_  = width;
        height_ = height;

        for (auto& it : viewports_) {
            it->onRenderTargetResize(*this, width_, height_);
        }
    }

    unsigned int RenderTarget::width() const noexcept {
        return width_;
    }

    unsigned int RenderTarget::height() const noexcept {
        return height_;
    }

} // namespace helios::rendering

