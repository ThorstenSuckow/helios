module;

#include <string>

export module helios.window.Window;

import helios.util.Guid;
import helios.window.WindowConfig;
import helios.math.types;
import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::window::Window"
export namespace helios::window {

    /**
     * @brief Abstract base class representing a generic window.
     *
     * This class provides a common interface for window management
     * without depending on the underlying native window implementation.
     *
     * A Window can be uniquely identified via its Guid.
     *
     */
    class Window {

    private:
        util::Guid guid_;

    protected:

        /**
         * @brief The const reference to the logger used with this Adapter.
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);

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
         * @brief The viewport configuration of the window, in terms of (x, y, width, height)
         */
        math::vec4i viewport_;

    public:
        virtual ~Window() = default;

        /**
         * @brief Constructs a new Window based on the provided configuration.
         *
         * Initializes basic properties such as width, height and the viewport
         * of the window.
         *
         * @param cfg A const ref to the window configuration used for this instance.
         */
        explicit Window(const WindowConfig& cfg);


        /**
         * @brief Checks if the window has received a close request.
         *
         * Implementing APIs should consider this flag in constant intervals
         * (e.g. each frame) to determine whether this window should be closed.
         *
         * @return bool True if the window should be closed, otherwise false.
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
         * @return A const ref to the Guid of this window.
         */
        [[nodiscard]] const util::Guid& guid() const noexcept;


        /**
         * @brief Shows the underlying native window.
         *
         * Derived classes must implement the platform-specific logic to
         * show this window.
         *
         * @return true if showing the window succeeded, otherwise false.
         *
         * @throws std::runtime_error if the window is already shown or
         * couldn't be created
         */
        virtual bool show() noexcept = 0;


        /**
         * @brief Advise the rendering system to swap the front and back buffers.
         *
         * Makes the back buffers content visible on the screen.
         * Derived classes must implement the platform-specific buffer mechanisms.
         */
        virtual void swapBuffers() const noexcept = 0;


        /**
         * @brief Poll this window for window related events.
         *
         * This method processes pending events form the native window system.
         * Implementing APIs should call this method in constant intervals, e.g.
         * once per frame.
         */
        virtual void pollEvents() const noexcept = 0;


        /**
         * @brief Returns the current with of this window.
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
         * @brief Returns the viewport configuration used with this window,
         * encoded in a `vec4i`, encoding x, y, width, height.
         *
         * @return A constant reference to the `vec4i` representing this window's viewport.
         */
        [[nodiscard]] const math::vec4i& viewport() const noexcept;


        /**
         * @brief Compares two window instances for equality.
         *
         * This implementation treats two windows as equal if their
         * guids are of the same value.
         *
         * @param win The window that should be compared for equality with this window.
         *
         * @return true if both Windows are treated as equal, otherwise false.
         */
        virtual bool operator==(const Window& win) const noexcept;
    };

}
