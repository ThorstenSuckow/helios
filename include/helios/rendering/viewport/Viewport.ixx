/**
 * @file Viewport.ixx
 *
 * @brief Defines the Viewport class, which represents a rectangular area for rendering within a Framebuffer.
 */
module;

#include <cassert>
#include <format>
#include <memory>


export module helios.rendering.viewport.Viewport;

import helios.rendering.viewport.ViewportSnapshot;

import helios.rendering.ClearFlags;
import helios.core.types;
import helios.rendering.viewport.types.ViewportId;
import helios.math.types;
import helios.scene.CameraSceneNode;
import :FramebufferFwd;
import helios.util.log.LogManager;
import helios.util.log.Logger;

#define HELIOS_LOG_SCOPE "helios::rendering::viewport::Viewport"
export namespace helios::rendering::viewport {


    /**
     * @brief A passkey used to establish a parent-child relationship between a Framebuffer and a Viewport.
     *
     * This struct uses the passkey idiom to restrict the calling of `Viewport::setFramebuffer` to
     * friend classes (specifically `helios::rendering::Framebuffer`), ensuring that the ownership
     * hierarchy is managed correctly.
     *
     * @see `Viewport::setFramebuffer`
     * @see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p2893r3.html#passkey-idiom
     */
    struct ViewportKey {
    private:
        friend class helios::rendering::framebuffer::Framebuffer;
        ViewportKey() = default;
    };

    /**
     * @brief Represents a rectangular area within a Framebuffer where a scene is rendered.
     *
     * A Viewport defines the 2D rectangle into which a 3D scene is projected. Its dimensions are specified
     * in normalized coordinates relative to its parent `Framebuffer`. It is associated with a `CameraSceneNode`,
     * which provides the view and projection matrices. The viewport automatically updates the CameraSceneNode
     * associated camera's aspect ratio when its parent `Framebuffer` is resized.
     *
     * Since a Viewport only stores normalized values for its location and dimensions relative to the owning Framebuffer,
     * rendering APIs should query the size of the Framebuffer when a viewport is about to be
     * rendered, allowing them to compute the final screen coordinates and dimensions correctly.
     *
     * @see helios::rendering::Framebuffer
     * @see helios::scene::CameraSceneNode
     * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml
     */
    class Viewport {
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    private:
        /**
         * @brief Pointer to the parent Framebuffer.
         */
        const helios::rendering::framebuffer::Framebuffer* framebuffer_ = nullptr;

        /**
         * @brief Bitmask of buffers to clear.
         *
         * Defaults to ClearFlags::Color.
         */
        int clearFlags_ = std::to_underlying(ClearFlags::Color);

        /**
         * @brief Color used when clearing the color buffer. Defaults to "black", i.e. no color at all.
         */
        helios::math::vec4f clearColor_ = helios::math::vec4f(0.0f, 0.0f, 0.0f, 1.0f);

        /**
         * @brief The x-offset of the viewport, normalized to [0, 1].
         */
        float x_ = 0.0f;
        /**
         * @brief The y-offset of the viewport, normalized to [0, 1].
         */
        float y_ = 0.0f;

        /**
         * @brief The width of the viewport, normalized to [0, 1].
         */
        float width_ = 1.0f;
        /**
         * @brief The height of the viewport, normalized to [0, 1].
         */
        float height_ = 1.0f;

        /**
         * @brief Size of the parent Framebuffer in pixels.
         */
        helios::math::vec2ui framebufferSize_{};

        /**
         * @brief The cameraSceneNode associated as the main camera with this viewport.
         */
        helios::scene::CameraSceneNode* cameraSceneNode_ = nullptr;

        /**
         * @brief Flag indicating if cached dimensions are dirty.
         */
        mutable bool needsUpdate_ = true;

        /**
         * @brief Cached dimensions [x, y, width, height].
         */
        mutable helios::math::vec4f bounds_;

        /**
         * @brief The z-index for this viewport.
         *
         * The z-index represents the order the Viewport gets rendered.
         * A higher z-index means that the viewport gets rendered after viewports with a lower z-index
         * are rendered.
         */
        int zIndex_ = 0;


        /**
         * @brief Cached absolute bounds [x, y, width, height] in pixels.
         */
        mutable helios::math::vec4f absoluteBounds_{};

        /**
         * @brief Updates the cached dimensions vector if it is dirty.
         */
        void updateCache() const noexcept {

            if (!needsUpdate_) {
                return;
            }

            bounds_[0] = x_;
            bounds_[1] = y_;
            bounds_[2] = width_;
            bounds_[3] = height_;

            absoluteBounds_ = {
            bounds_[0] * framebufferSize_[0],
            bounds_[1] * framebufferSize_[1],
            bounds_[2] * framebufferSize_[0],
            bounds_[3] * framebufferSize_[1]
            };

            needsUpdate_ = false;
        }


        /**
         * @brief Updates this Viewport's CameraSceneNode and its associated Camera based on the dimension of the Framebuffer.
         *
         * @param width The new width of the Framebuffer in pixels.
         * @param height The new height of the Framebuffer in pixels.
         *
         * This method should be called internally whenever the dimensions of the owning
         * Framebuffer change.
         */
        void updateCamera(unsigned int framebufferWidth, unsigned int framebufferHeight) noexcept {
            assert(framebuffer_ && "No Framebuffer available for updateCamera()");

            if (!cameraSceneNode_) {
                logger_.warn("updateCamera: Viewport was not configured with a CameraSceneNode, nothing to do here.");
                return;
            }

            cameraSceneNode_->camera().onResize(
                static_cast<float>(framebufferWidth) * width_,
                static_cast<float>(framebufferHeight) * height_
            );
        }

    public:

        Viewport(const Viewport& other) = delete;
        Viewport operator=(const Viewport& other) = delete;

        Viewport(Viewport&& other) noexcept = default;
        Viewport& operator=(Viewport&& other) noexcept  = default;

        /**
         * @brief Constructs a Viewport with normalized bounds set to (0.0f, 0.0f, 1.0f, 1.0f) and z-index 0.
         */
        Viewport() noexcept :  x_(0.0f), y_(0.0f), width_(1.0f), height_(1.0f), zIndex_(0) {
            needsUpdate_ = true;
        }


        /**
         * @brief Constructs a Viewport with specified normalized dimensions.
         *
         * @param x The normalized horizontal offset [0, 1].
         * @param y The normalized vertical offset [0, 1].
         * @param width The normalized width [0, 1].
         * @param height The normalized height [0, 1].
         * @param zIndex The z-index that determines the rendering order.
         */
        explicit Viewport(
            float x, float y, float width, float height, const int zIndex = 0
        ) noexcept
            :
            zIndex_(zIndex) {
            setBounds(x, y, width, height);
        }

        /**
         * @brief Gets the parent Framebuffer.
         *
         * If the returned value is a nullptr, this Viewport is not owned by any Framebuffer.
         *
         * @return A const pointer to the parent Framebuffer, or `nullptr` if not set.
         */
        [[nodiscard]] const helios::rendering::framebuffer::Framebuffer* framebuffer() const noexcept {
            return framebuffer_;
        }


        /**
         * @brief Assigns a camera scene node to this viewport.
         *
         * The viewport uses the associated camera's projection matrix and the node's
         * computed view matrix for rendering. The camera's aspect ratio is automatically
         * updated when the parent Framebuffer is resized.
         *
         * @param cameraSceneNode A non-owning raw pointer to the `CameraSceneNode`.
         *        Must remain valid for the lifetime of this viewport.
         *
         * @return A reference to this viewport to allow fluent chaining.
         */
        Viewport& setCameraSceneNode(helios::scene::CameraSceneNode* cameraSceneNode) noexcept {
            cameraSceneNode_ = cameraSceneNode;
            return *this;
        }

        /**
         * @brief Gets the camera scene node associated with this viewport.
         *
         * @return A const pointer to the associated `CameraSceneNode`, or `nullptr` if none is set.
         */
        [[nodiscard]] const helios::scene::CameraSceneNode* cameraSceneNode() const noexcept {
            return cameraSceneNode_;
        }

        /**
         * @brief Sets the parent Framebuffer for this viewport.
         *
         * This function can only be called by classes that can construct a `ViewportKey`,
         * effectively restricting its use to the `Framebuffer` class.
         *
         * @param framebuffer A pointer to the parent Framebuffer.
         * @param key A `ViewportKey` instance, required for authorization.
         *
         * @return A reference to this viewport to allow fluent chaining.
         *
         * @see updateCamera()
         *
         * @todo The Viewport should observe the Framebuffer for state changes (e.g., resize).
         */
        Viewport& setFramebuffer(const helios::rendering::framebuffer::Framebuffer* framebuffer, ViewportKey key) noexcept {
            framebuffer_ = framebuffer;
            return *this;
        }

        /**
         * @brief Gets the cached dimensions of the viewport.
         *
         * @return A const reference to a vec4f containing [x, y, width, height].
         */
        [[nodiscard]] const helios::math::vec4f& bounds() const noexcept {
            updateCache();
            return bounds_;
        }

        /**
         * @brief Gets the cached absolute bounds of the viewport in pixels.
         *
         * @return A const reference to a vec4f containing [x, y, width, height] in pixels.
         */
        [[nodiscard]] const helios::math::vec4f& absoluteBounds() const noexcept {
            updateCache();
            return absoluteBounds_;
        }


        /**
         * @brief Sets the normalized bounds of the viewport.
         *
         * @param x The normalized horizontal offset [0, 1].
         * @param y The normalized vertical offset [0, 1].
         * @param width The normalized width [0, 1].
         * @param height The normalized height [0, 1].
         */
        void setBounds(
            float x, float y, float width, float height
        ) noexcept {
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

        /**
         * @brief Gets the clear flags for this viewport.
         *
         * @return An integer bitmask representing the `ClearFlags`.
         */
        [[nodiscard]] int clearFlags() const noexcept {
            return clearFlags_;
        }

        /**
         * @brief Gets the color used for clearing the color buffer.
         *
         * @return A const reference to the clear color.
         */
        [[nodiscard]] const helios::math::vec4f& clearColor() const noexcept {
            return clearColor_;
        }

        /**
         * @brief Sets the clear flags for this viewport.
         *
         * @param clearFlags An integer bitmask created from `ClearFlags` enum values.
         *
         * @return A reference to this viewport to allow fluent chaining.
         */
        Viewport& setClearFlags(int clearFlags) noexcept {
            clearFlags_ = clearFlags;
            return *this;
        }

        /**
         * @brief Specifies the RGBA values used when clearing color buffers.
         *
         * Values are automatically clamped to the range [0, 1].
         *
         * @param color The color to use for clearing.
         *
         * @return A reference to this viewport to allow fluent chaining.
         */
        Viewport& setClearColor(const helios::math::vec4f& color) noexcept {
            clearColor_ = color;
            return *this;
        }

        /**
         * @brief Sets the z-index that controls rendering order relative to sibling viewports.
         *
         * @param zIndex The new z-index value; higher values are rendered later.
         *
         * @return A reference to this viewport to allow fluent chaining.
         */
        Viewport& setZIndex(int zIndex) noexcept {
            zIndex_ = zIndex;
            return *this;
        }


        /**
         * @brief Notifies the viewport of a change in the parent Framebuffer's dimensions.
         *
         * This method is called by the parent `Framebuffer` when it is resized. The viewport
         * uses the new dimensions to calculate the correct aspect ratio for its associated camera.
         *
         * @param width The new width of the Framebuffer in pixels.
         * @param height The new height of the Framebuffer in pixels.
         *
         * @see updateCamera()
         */
         void onFramebufferResize(const unsigned int width, const unsigned int height) noexcept {
            framebufferSize_ = {width, height};
            needsUpdate_ = true;
            updateCamera(width, height);
         }
    };
} // namespace helios::rendering::viewport
