/**
 * @file BasicWindowRenderingController.ixx
 * @brief Controller implementation for basic window rendering management.
 */
module;

#include <string>
#include <format>

export module helios.app.controller.BasicWindowRenderingController;

import helios.app.controller.WindowRenderingController;

import helios.rendering.RenderingDevice;
import helios.window.event.FrameBufferResizeEvent;
import helios.event.Dispatcher;
import helios.window.Window;

export namespace helios::app::controller {

    /**
     * @brief Basic implementation of a window rendering controller.
     *
     * This controller handles framebuffer resize events and manages rendering
     * for a single window instance.
     */
    class BasicWindowRenderingController final : public WindowRenderingController {

    private:
        /**
         * @brief Reference to the window managed by this controller.
         */
        helios::window::Window& window_;

    public:
        /**
         * @brief Constructs a new BasicWindowRenderingController instance.
         *
         * @param window Reference to the window to be managed by this controller.
         */
        explicit BasicWindowRenderingController(helios::window::Window& window):
        window_(window)
        {}

        /**
         * @brief Handles framebuffer resize events.
         *
         * @param e The framebuffer resize event containing the new dimensions.
         */
        void onFrameBufferResize(const helios::window::event::FrameBufferResizeEvent& e) override {
            logger_.debug(std::format("onFrameBufferResize ({0}, {1})", e.width, e.height));
            window_.renderTarget().setSize(e.width, e.height);
        }

        /**
         * @brief Registers this controller to the framebuffer resize event.
         *
         * @param dispatcher The event dispatcher to subscribe to.
         *
         * @see onFrameBufferResize
         */
        void subscribeTo(helios::event::Dispatcher& dispatcher) override {
            logger_.debug("Subscribing to dispatcher.");
            dispatcher.subscribe<helios::window::event::FrameBufferResizeEvent>(
                [this](const helios::window::event::FrameBufferResizeEvent& e) {
                    onFrameBufferResize(e);
                }
            );
        }

        /**
         * @brief Initializes this controller.
         *
         * @return True if initialization succeeded, otherwise false.
         */
        bool init() override {
            return true;
        }

        [[nodiscard]] std::string toString() const noexcept override {
            return "BasicWindowController";
        }
    };


}