module;

#include <cassert>
#include <format>


module helios.rendering.Viewport;


import helios.rendering.RenderTarget;



namespace helios::rendering {

    void Viewport::updateBounds() const noexcept {
        bounds_[0] = x_;
        bounds_[1] = y_;
        bounds_[2] = width_;
        bounds_[3] = height_;

        needsUpdate_ = false;
    }

    void Viewport::updateCamera() noexcept {
        assert(renderTarget_ && "No RenderTarget available for updateCamera()");

        if (!cameraSceneNode_) {
            logger_.warn("updateCamera: Viewport was not configured with a CameraSceneNode, nothing to do here.");
            return;
        }

        const float w = static_cast<float>(renderTarget_->width()) * width_;
        const float h = static_cast<float>(renderTarget_->height()) * height_;

        if (h > 0.0f) {
            logger_.info(std::format("Setting aspect ratio {0}/{1}", w, h));
            cameraSceneNode_->camera().setAspectRatio(w / h);
        } else {
            logger_.warn("updateCamera: cannot set aspect ratio, height is 0");
        }
    }

    Viewport::Viewport() noexcept : x_(0.0f), y_(0.0f), width_(1.0f), height_(1.0f), zIndex_(0) {
        needsUpdate_ = true;
    }

    Viewport::Viewport(const float x, const float y, const float width, const float height, int zIndex) noexcept
        : zIndex_(zIndex) {
        setBounds(x, y, width, height);
    }

    [[nodiscard]] const helios::rendering::RenderTarget* Viewport::renderTarget() const noexcept {
        return renderTarget_;
    }

    Viewport& Viewport::setCameraSceneNode(helios::scene::CameraSceneNode* cameraSceneNode) noexcept {
        cameraSceneNode_ = cameraSceneNode;
        return *this;
    }

    [[nodiscard]] const helios::scene::CameraSceneNode* Viewport::cameraSceneNode() const noexcept {
        return cameraSceneNode_;
    }

    Viewport& Viewport::setRenderTarget(const helios::rendering::RenderTarget* renderTarget, ViewportKey key) noexcept {
        renderTarget_ = renderTarget;
        updateCamera();
        return *this;
    }

    [[nodiscard]] const helios::math::vec4f& Viewport::bounds() const noexcept {
        if (needsUpdate_) {
            updateBounds();
        }
        return bounds_;
    }

    void Viewport::setBounds(float x, float y, float width, float height) noexcept {
        x_ = x;
        y_ = y;
        width_ = width;
        height_ = height;

        needsUpdate_ = true;

        assert((x_ >= 0.0f && x_ <= 1.0f) && "setBounds received unexpected value for x");
        assert((y_ >= 0.0f && y_ <= 1.0f) && "setBounds received unexpected value for y");
        assert((width_ >= 0.0f && width_ <= 1.0f) && "setBounds received unexpected value for width");
        assert((height_ >= 0.0f && height_ <= 1.0f) && "setBounds received unexpected value for height");
    }

    [[nodiscard]] int Viewport::clearFlags() const noexcept {
        return clearFlags_;
    }

    [[nodiscard]] const helios::math::vec4f& Viewport::clearColor() const noexcept {
        return clearColor_;
    }

    Viewport& Viewport::setClearFlags(int clearFlags) noexcept {
        clearFlags_ = clearFlags;
        return *this;
    }

    Viewport& Viewport::setClearColor(const helios::math::vec4f& color) noexcept {
        clearColor_ = color;
        return *this;
    }

    Viewport& Viewport::setZIndex(const int zIndex) noexcept {
        zIndex_ = zIndex;
        return *this;
    }

    void Viewport::onRenderTargetResize(const helios::rendering::RenderTarget& source, const unsigned int width,
                                        const unsigned int height) noexcept {
        updateCamera();
    }

} // namespace helios::rendering
