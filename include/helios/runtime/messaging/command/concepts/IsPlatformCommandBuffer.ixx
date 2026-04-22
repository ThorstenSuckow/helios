/**
 * @file IsCommandBufferLike.ixx
 * @brief Concept constraining types usable as command buffers.
 */
module;

#include <concepts>

export module helios.runtime.messaging.command.concepts.IsPlatformCommandBuffer;

import helios.runtime.messaging.command.PlatformCommandBuffer;

using namespace helios::runtime::messaging::command;
export namespace helios::runtime::messaging::command::concepts {

    /**
     * @brief Constrains types that can serve as concrete command buffers.
     *
     * @details Requires `flush(GameWorld&, UpdateContext&) noexcept` and
     * `clear() noexcept` (via HasClear). Types satisfying this concept
     * can be wrapped by the type-erased CommandBuffer wrapper.
     *
     * @tparam T The type to constrain.
     *
     * @see CommandBuffer
     * @see CommandBufferRole
     * @see HasClear
     */
    template<class T>
    concept IsPlatformCommandBuffer = std::same_as<T, PlatformCommandBuffer>;
}
