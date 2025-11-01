module;

#include <cstdint>

export module helios.window.event.FrameBufferResizeEvent;

import helios.window.event.WindowEvent;

import helios.util.Guid;

using namespace helios::window::event;

export namespace helios::window::event {

    /**
     * @brief Event representing a change in the framebuffer's size.
     *
     * The event should be triggered whenever a change in the underlying
     * framebuffer of a window is detected, indicating that the rendering context
     * (e.g. OpenGL) might need adjustments. FramebufferResizeEvents
     * carry the with and the height of the framebuffer.
     */
    class FrameBufferResizeEvent final : public WindowEvent {

    public:
        /**
         * @brief The width of the framebuffer in pixels.
         */
        const int width;


        /**
         * @brief The height of the framebuffer in pixels
         */
        const int height;


        /**
         * @brief Constructs a `FrameBufferResizeEvent`.
         *
         * @param sourceGuid The Guid of the source of the event.
         * @param width The new width of the framebuffer.
         * @param height The new height of the framebuffer.
         */
        FrameBufferResizeEvent(
            const util::Guid sourceGuid, const int width, const int height
        ) noexcept;


        /**
         * @brief Constructs a `FrameBufferResizeEvent`.
         *
         * @param sourceGuid The Guid of the source of the event.
         * @param width The new width of the framebuffer.
         * @param height The new height of the framebuffer.
         * @param tag A `uint64_t` tag for identifying/categorizing the event.
         */
        FrameBufferResizeEvent(
            const util::Guid sourceGuid,
            const int width, const int height, const uint64_t tag
        ) noexcept;

    };

}