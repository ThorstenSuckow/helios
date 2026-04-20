/**
 * @file Framebuffer.ixx
 * @brief Defines the Framebuffer class, which encapsulates a destination for rendering operations.
 */
module;

#include <vector>

export module helios.rendering.framebuffer.Framebuffer;



export namespace helios::rendering::framebuffer {


    class Framebuffer {
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
         * @brief A list of viewports owned by this render target.
         *
         * @todo The list should be sorted after a meaningful key, like the viewport's z-Index.
         */
      //  std::vector<helios::rendering::viewport::types::ViewportHandle> viewports_;

        bool isDirty_ = true;

    public:
        /**
         * @brief Default constructor.
         */
        Framebuffer() = default;

        Framebuffer(const Framebuffer& other) = delete;
        Framebuffer& operator=(const Framebuffer& other) = delete;

        Framebuffer(Framebuffer&& other) noexcept =  default;
        Framebuffer& operator=(Framebuffer&& other) noexcept = default;

        explicit Framebuffer(
            const unsigned int width,
            const unsigned int height)
            : width_(width),
              height_(height) {}


        void setSize(const unsigned int width, const unsigned int height) noexcept {

            if (width == width_ && height == height_) {
                return;
            }

            width_  = width;
            height_ = height;

            isDirty_ = true;
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

        [[nodiscard]] bool isDirty() const noexcept {
            return isDirty_;
        }

        void clearDirty() noexcept {
            isDirty_ = false;
        }

    };

} // namespace helios::rendering::framebuffer
