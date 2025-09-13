module;

#include <string>

export module helios.platform.core:Window;

import helios.util.Guid;
import :WindowConfig;

export namespace helios::platform {

    class Window {

    private:
        util::Guid guid_;

    protected:
        int width_;
        int height_;
        std::string title_;

    public:
        explicit Window(const WindowConfig& cfg) :
            width_(cfg.width), height_(cfg.height), title_(cfg.title),
            guid_(util::Guid::generate()){};

        virtual ~Window() = default;
        [[nodiscard]] virtual bool shouldClose() const = 0;

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
         * @return
         *
         * throws std::runtime_error if the window is already shown or
         * couldn't be created
         */
        virtual Window& show() = 0;

        /**
         * Advise to swap front- and backbuffer.
         *
         * @return
         */
        virtual Window& swapBuffers() = 0;

        /**
         * Poll this window for window related events.
         * @return
         */
        virtual Window& pollEvents() = 0;

    };

}
