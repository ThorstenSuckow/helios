/**
 * @file IsCommandBufferLike.ixx
 * @brief Concept constraining types usable as command buffers.
 */
module;

#include <concepts>

export module helios.engine.common.concepts.IsCommandBufferLike;

import helios.engine.common.concepts.HasTag;
import helios.engine.common.concepts.HasClear;


import helios.runtime.world.GameWorldFwd;
import helios.runtime.world.UpdateContextFwd;

import helios.engine.common.tags.CommandBufferRole;

import helios.engine.common.tags.CommandBufferRole;

using namespace helios::engine::common::tags;
using namespace helios::runtime::world;
export namespace helios::engine::common::concepts {

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
    concept IsCommandBufferLike = requires(T& t, GameWorld& gameWorld, UpdateContext& updateContext) {
        {t.flush(gameWorld, updateContext) } -> std::same_as<void>;
    } && HasClear<T> && HasTag<T, CommandBufferRole>;
}
