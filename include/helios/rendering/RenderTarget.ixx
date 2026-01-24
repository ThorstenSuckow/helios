/**
 * @file RenderTarget.ixx
 * @brief Defines the RenderTarget class, which encapsulates a destination for rendering operations.
 */
module;

#include <memory>
#include <vector>
#include <stdexcept>

export module helios.rendering.RenderTarget;

import helios.rendering.Viewport;


export namespace helios::rendering {

    /**
     * @brief Represents a destination for rendering operations, such as a window's framebuffer.
     *
     * A RenderTarget encapsulates a buffer (e.g., the default framebuffer or an off-screen texture)
     * into which a scene can be rendered. It manages one or more `Viewport` objects, which define
     * specific rectangular areas within the target.
     *
     * When the RenderTarget is resized, it notifies all its associated viewports so they can update
     * their states accordingly (e.g., camera aspect ratio).
     *
     * @todo This implementation currently represents only the **default framebuffer**. Future extensions
     *       should allow it to represent arbitrary framebuffer objects (FBOs), using indices.
     * @see glGenFramebuffers
     */
    class RenderTarget {
    private:
        /**
         * @brief The width of the render target in pixels.
         */
        unsigned int width_ = 0;

        /**
         * @brief The height of the render target in pixels.
         */
        unsigned int height_ = 0;

        /**
         * @brief Passkey instance to authorize `Viewport::setRenderTarget`.
         */
        helios::rendering::ViewportKey viewportKey_{};

        /**
         * @brief A list of viewports owned by this render target.
         *
         * @todo The list should be sorted after a meaningful key, like the viewport's z-Index.
         */
        std::vector<std::shared_ptr<helios::rendering::Viewport>> viewports_;

    public:
        /**
         * @brief Default constructor.
         */
        RenderTarget() = default;

        /**
         * @brief Constructs a RenderTarget with an initial viewport and dimensions.
         *
         * @param viewport The default viewport for this RenderTarget. The viewport defines the
         *                 rectangular area where the image is drawn.
         * @param width The initial width of the render target in pixels.
         * @param height The initial height of the render target in pixels.
         *
         * @throws std::invalid_argument if viewport is a nullptr.
         *
         * @see addViewport()
         */
        explicit RenderTarget(
            std::shared_ptr<helios::rendering::Viewport> viewport,
            unsigned int width = 0,
            unsigned int height = 0)
            : width_(width),
              height_(height) {
            if (!viewport) {
                throw std::invalid_argument("Constructor received a null shared pointer");
            }

            addViewport(std::move(viewport));
        }

        /**
         * @brief Adds a viewport to this render target and establishes a parent-child relationship.
         *
         * Makes sure the viewport is informed about any possible bounds updates by
         * calling onRenderTargetResize afterwards.
         *
         * @param viewport A shared pointer to the `Viewport` to be added.
         *
         * @return The newly added viewport as a shared pointer.
         *
         * @throws std::invalid_argument if viewport is a nullptr, or if the viewport already
         *         has a parent RenderTarget.
         */
        std::shared_ptr<helios::rendering::Viewport> addViewport(
            std::shared_ptr<helios::rendering::Viewport> viewport) {
            if (!viewport) {
                throw std::invalid_argument("addViewport() received a null shared pointer");
            }

            if (viewport->renderTarget()) {
                throw std::invalid_argument("Viewport already belongs to a RenderTarget.");
            }

            viewport->setRenderTarget(&*this, viewportKey_);
            viewports_.emplace_back(viewport);

            return viewport;
        }

        /**
         * @brief Resizes this RenderTarget to the specified dimensions and propagates the change to every viewport.
         *
         * @param width The new width of the render target in pixels.
         * @param height The new height of the render target in pixels.
         *
         * @todo A LayoutManager could be introduced to manage the arrangement of multiple viewports.
         *
         * @see helios::rendering::Viewport::onRenderTargetResize()
         */
        void setSize(unsigned int width, unsigned int height) noexcept {
            width_  = width;
            height_ = height;

            for (auto& it : viewports_) {
                it->onRenderTargetResize(width_, height_);
            }
        }

        /**
         * @brief Gets the width of the render target.
         *
         * @return The width in pixels.
         */
        [[nodiscard]] unsigned int width() const noexcept {
            return width_;
        }

        /**
         * @brief Gets the height of the render target.
         *
         * @return The height in pixels.
         */
        [[nodiscard]] unsigned int height() const noexcept {
            return height_;
        }

    };

} // namespace helios::rendering
