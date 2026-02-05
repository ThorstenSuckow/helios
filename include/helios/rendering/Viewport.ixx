/**
 * @file Viewport.ixx
 *
 * @brief Defines the Viewport class, which represents a rectangular area for rendering within a RenderTarget.
 */
module;

#include <cassert>
#include <format>
#include <memory>


export module helios.rendering.Viewport;

import helios.rendering.ViewportSnapshot;

import helios.rendering.ClearFlags;
import helios.core.types;
import helios.engine.core.data.ViewportId;
import helios.math.types;
import helios.scene.CameraSceneNode;
import :RenderTargetFwd;
import helios.util.log.LogManager;
import helios.util.log.Logger;

#define HELIOS_LOG_SCOPE "helios::rendering::Viewport"
export namespace helios::rendering {


    
    
    /**
     * @brief A passkey used to establish a parent-child relationship between a RenderTarget and a Viewport.
     *
     * This struct uses the passkey idiom to restrict the calling of `Viewport::setRenderTarget` to
     * friend classes (specifically `helios::rendering::RenderTarget`), ensuring that the ownership
     * hierarchy is managed correctly.
     *
     * @see `Viewport::setRenderTarget`
     * @see https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p2893r3.html#passkey-idiom
     */
    struct ViewportKey {
    private:
        friend class helios::rendering::RenderTarget;
        ViewportKey() = default;
    };

    /**
     * @brief Represents a rectangular area within a RenderTarget where a scene is rendered.
     *
     * A Viewport defines the 2D rectangle into which a 3D scene is projected. Its dimensions are specified
     * in normalized coordinates relative to its parent `RenderTarget`. It is associated with a `CameraSceneNode`,
     * which provides the view and projection matrices. The viewport automatically updates the CameraSceneNode
     * associated camera's aspect ratio when its parent `RenderTarget` is resized.
     *
     * Since a Viewport only stores normalized values for its location and dimensions relative to the owning RenderTarget,
     * rendering APIs should query the size of the RenderTarget when a viewport is about to be
     * rendered, allowing them to compute the final screen coordinates and dimensions correctly.
     *
     * @see helios::rendering::RenderTarget
     * @see helios::scene::CameraSceneNode
     * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml
     */
    class Viewport {
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

    private:
        /**
         * @brief Pointer to the parent RenderTarget.
         */
        const helios::rendering::RenderTarget* renderTarget_ = nullptr;

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
         * @brief Size of the parent RenderTarget in pixels.
         */
        helios::math::vec2ui renderTargetSize_{};

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
         * @brief Unique identifier for this viewport.
         */
        const helios::engine::core::data::ViewportId viewportId_{helios::core::types::no_init};

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
            bounds_[0] * renderTargetSize_[0],
            bounds_[1] * renderTargetSize_[1],
            bounds_[2] * renderTargetSize_[0],
            bounds_[3] * renderTargetSize_[1]
            };

            snapshot_ = {viewportId_, bounds_, absoluteBounds_};

            needsUpdate_ = false;
        }

        /**
         * @brief Cached viewport snapshot for efficient access.
         */
        mutable ViewportSnapshot snapshot_{};

        /**
         * @brief Updates this Viewport's CameraSceneNode and its associated Camera based on the dimension of the RenderTarget.
         *
         * @param width The new width of the RenderTarget in pixels.
         * @param height The new height of the RenderTarget in pixels.
         *
         * This method should be called internally whenever the dimensions of the owning
         * RenderTarget change.
         */
        void updateCamera(unsigned int renderTargetWidth, unsigned int renderTargetHeight) noexcept {
            assert(renderTarget_ && "No RenderTarget available for updateCamera()");

            if (!cameraSceneNode_) {
                logger_.warn("updateCamera: Viewport was not configured with a CameraSceneNode, nothing to do here.");
                return;
            }

            cameraSceneNode_->camera().onResize(
                static_cast<float>(renderTargetWidth) * width_,
                static_cast<float>(renderTargetHeight) * height_
            );
        }

    public:
        /**
         * @brief Constructs a Viewport with normalized bounds set to (0.0f, 0.0f, 1.0f, 1.0f) and z-index 0.
         */
        Viewport() noexcept : x_(0.0f), y_(0.0f), width_(1.0f), height_(1.0f), zIndex_(0) {
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
            float x, float y, float width, float height,
            const helios::engine::core::data::ViewportId viewportId = helios::engine::core::data::ViewportId{helios::core::types::no_init},
            const int zIndex = 0
        ) noexcept
            :
            viewportId_(viewportId),
            zIndex_(zIndex) {
            setBounds(x, y, width, height);
        }

        /**
         * @brief Gets the parent RenderTarget.
         *
         * If the returned value is a nullptr, this Viewport is not owned by any RenderTarget.
         *
         * @return A const pointer to the parent RenderTarget, or `nullptr` if not set.
         */
        [[nodiscard]] const helios::rendering::RenderTarget* renderTarget() const noexcept {
            return renderTarget_;
        }

        /**
         * @brief Returns the unique identifier for this viewport.
         *
         * @return The ViewportId assigned to this viewport.
         */
        [[nodiscard]] helios::engine::core::data::ViewportId viewportId() const noexcept {
            return viewportId_;
        }

        /**
         * @brief Assigns a camera scene node to this viewport.
         *
         * The viewport uses the associated camera's projection matrix and the node's
         * computed view matrix for rendering. The camera's aspect ratio is automatically
         * updated when the parent RenderTarget is resized.
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
         * @brief Sets the parent RenderTarget for this viewport.
         *
         * This function can only be called by classes that can construct a `ViewportKey`,
         * effectively restricting its use to the `RenderTarget` class.
         *
         * @param renderTarget A pointer to the parent RenderTarget.
         * @param key A `ViewportKey` instance, required for authorization.
         *
         * @return A reference to this viewport to allow fluent chaining.
         *
         * @see updateCamera()
         *
         * @todo The Viewport should observe the RenderTarget for state changes (e.g., resize).
         */
        Viewport& setRenderTarget(const helios::rendering::RenderTarget* renderTarget, ViewportKey key) noexcept {
            renderTarget_ = renderTarget;
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
         * @brief Returns an immutable snapshot of this viewport's current state.
         *
         * The snapshot contains the viewport ID and both normalized and absolute bounds.
         * Useful for passing viewport state to rendering systems without exposing the
         * mutable Viewport object.
         *
         * @return A ViewportSnapshot containing the current viewport state.
         */
        [[nodiscard]] ViewportSnapshot snapshot() const noexcept {
            updateCache();
            return snapshot_;
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
         * @brief Notifies the viewport of a change in the parent RenderTarget's dimensions.
         *
         * This method is called by the parent `RenderTarget` when it is resized. The viewport
         * uses the new dimensions to calculate the correct aspect ratio for its associated camera.
         *
         * @param width The new width of the RenderTarget in pixels.
         * @param height The new height of the RenderTarget in pixels.
         *
         * @see updateCamera()
         */
         void onRenderTargetResize(const unsigned int width, const unsigned int height) noexcept {
            renderTargetSize_ = {width, height};
            needsUpdate_ = true;
            updateCamera(width, height);
         }
    };
} // namespace helios::rendering
