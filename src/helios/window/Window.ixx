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

    class Window {

    private:
        util::Guid guid_;

    protected:

        /**
         * The const reference to the logger used with this Adapter.
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);


        int width_;
        int height_;
        std::string title_;
        math::vec4 viewport_;

    public:
        explicit Window(const WindowConfig& cfg) :
            width_(cfg.width), height_(cfg.height), title_(cfg.title),
            viewport_(cfg.viewport),
            guid_(util::Guid::generate()){};

        virtual ~Window() = default;
        [[nodiscard]] virtual bool shouldClose() const = 0;

        /**
         * Sets the close flag of this window to true.
         * Implementing APIs should consider this flag in each tick
         * to determine whether this window should be closed.
         */
        virtual void setShouldClose(bool close) = 0;


        /**
         * Returns the guid for this instance.
         *
         * @return util::Guid
         */
        [[nodiscard]] const util::Guid& guid() const noexcept {
            return guid_;
        }


        /**
         * Shows the underlying native window.
         *
         * @return true if showing the window succeeded, otherwise false.
         *
         * throws std::runtime_error if the window is already shown or
         * couldn't be created
         */
        virtual bool show() noexcept = 0;


        /**
         * Advise to swap front- and backbuffer.
         *
         * @return
         */
        virtual void swapBuffers() const noexcept = 0;


        /**
         * Poll this window for window related events.
         * @return
         */
        virtual void pollEvents() const noexcept = 0;


        [[nodiscard]] int width() const noexcept {
            return width_;
        }


        [[nodiscard]] int height() const noexcept {
            return width_;
        }

        /**
         * Returns the viewport used with this window, encoded in a vec4.
         *
         * @return
         */
        [[nodiscard]] const math::vec4& viewport() const noexcept {
            return viewport_;
        }


    };

}
