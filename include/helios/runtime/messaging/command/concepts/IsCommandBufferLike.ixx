/**
 * @file IsCommandBufferLike.ixx
 * @brief Concept constraining types usable as command buffers.
 */
module;

#include <concepts>

export module helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.runtime.concepts.HasTag;
import helios.runtime.concepts.HasClear;

import helios.runtime.world.UpdateContext;

import helios.runtime.messaging.command.tags.CommandBufferRole;


using namespace helios::runtime::tags;
using namespace helios::runtime::world;
using namespace helios::runtime::concepts;
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
    concept IsCommandBufferLike = requires(T& t, UpdateContext& updateContext) {
        {t.flush(updateContext) } -> std::same_as<void>;
    } && HasClear<T> && HasTag<T, CommandBufferRole>;
}
