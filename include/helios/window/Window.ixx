/**
 * @file Window.ixx
 * @brief Abstract window interface used by the helios framework.
 */
module;

#include <memory>
#include <string>

export module helios.window.Window;

import helios.util.Guid;
import helios.window.WindowConfig;
import helios.math.types;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.rendering.RenderTarget;
import helios.rendering.Viewport;

#define HELIOS_LOG_SCOPE "helios::window::Window"
export namespace helios::window {

    /**
     * @brief Abstract base class representing a generic window.
     *
     * This class provides a common interface for window management
     * without depending on the underlying native window implementation.
     *
     * A Window can be uniquely identified via its Guid.
     */
    class Window {

    private:
        util::Guid guid_;

    protected:

        /**
         * @brief The logger used with this Window instance.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief The current width of the window.
         */
        int width_;

        /**
         * @brief The current height of the window.
         */
        int height_;

        /**
         * @brief The title of the window.
         */
        std::string title_;

        /**
         * @brief Aspect ratio numerator.
         *
         * Set to anything other than 0 together with aspectRatioDenom to enforce aspect ratio.
         */
        int aspectRatioNumer_ = 0;

        /**
         * @brief Aspect ratio denominator.
         *
         * Set to anything other than 0 together with aspectRatioNumer to enforce aspect ratio.
         */
        int aspectRatioDenom_ = 0;

        /**
         * @brief The default render target for this Window.
         * The default RenderTarget represents the default framebuffer, e.g. the "0" FBO in OpenGL.
         */
        std::unique_ptr<helios::rendering::RenderTarget> renderTarget_;

    public:
        virtual ~Window() = default;

        // not copyable
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        // moveable
        Window(Window&&) noexcept = default;
        Window& operator=(Window&&) noexcept = default;

        /**
         * @brief Constructs a new Window based on the provided configuration.
         *
         * Initializes basic properties such as width, height and the viewport
         * of the window.
         *
         * @param renderTarget
         * @param cfg A const ref to the window configuration used for this instance.
         *
         * @throws std::invalid_argument if renderTarget is a nullptr
         */
        explicit Window(
            std::unique_ptr<helios::rendering::RenderTarget> renderTarget,
            const WindowConfig& cfg
        );


        /**
         * @brief Checks if the window has received a close request.
         *
         * Implementing APIs should consider this flag at regular intervals
         * (e.g. each frame) to determine whether this window should be closed.
         *
         * @return true if the window should be closed, otherwise false.
         *
         * @see setShouldClose()
         */
        [[nodiscard]] virtual bool shouldClose() const = 0;


        /**
         * @brief Sets the close flag of this window.
         *
         * @param close True to indicate this window should be closed,
         * false otherwise.
         *
         * @see shouldClose()
         */
        virtual void setShouldClose(bool close) = 0;


        /**
         * @brief Returns the unique guid for this window instance.
         *
         * @return A const reference to the Guid of this window.
         */
        [[nodiscard]] const util::Guid& guid() const noexcept;


        /**
         * @brief Shows the underlying native window.
         *
         * Derived classes must implement the platform-specific logic to
         * show this window.
         *
         * @return true if showing the window succeeded, otherwise false.
         */
        virtual bool show() noexcept = 0;


        /**
         * @brief Instructs the rendering system to swap the front and back buffers.
         *
         * Makes the back buffer's content visible on the screen.
         * Derived classes must implement the platform-specific buffer mechanism.
         */
        virtual void swapBuffers() const noexcept = 0;


        /**
         * @brief Poll this window for native window events.
         *
         * This method processes pending events from the native window system.
         * Implementing APIs should call this method at regular intervals, e.g.
         * once per frame.
         */
        virtual void pollEvents() const noexcept = 0;


        /**
         * @brief Returns the current width of this window.
         *
         * @return The current width of this window.
         */
        [[nodiscard]] int width() const noexcept;


        /**
         * @brief Returns the current height of this window.
         *
         * @return The current height of this window.
         */
        [[nodiscard]] int height() const noexcept;


        /**
         * @brief Returns the RenderTarget associated with this Window.
         *
         * @return A reference to the RenderTarget of this window.
         */
        [[nodiscard]] helios::rendering::RenderTarget& renderTarget() const noexcept;

        /**
         * @brief Adds the viewport to the underlying RenderTarget of this Window.
         *
         * @param viewport The Viewport instance to add to the RenderTarget owned by this Window.
         *
         * @return The Viewport added to the Window's RenderTarget as a shared pointer.
         */
        std::shared_ptr<helios::rendering::Viewport> addViewport(std::shared_ptr<helios::rendering::Viewport> viewport) const;

        /**
         * @brief Compares two window instances for equality.
         *
         * Two windows are considered equal if their GUIDs are equal.
         *
         * @param win The window to compare with this window.
         * @return true if both windows are equal, otherwise false.
         */
        virtual bool operator==(const Window& win) const noexcept;
    };

} // namespace helios::window
