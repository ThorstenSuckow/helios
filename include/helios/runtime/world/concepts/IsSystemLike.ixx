/**
 * @file IsSystemLike.ixx
 * @brief Concept constraining types eligible for system registration.
 */
module;

#include <concepts>

export module helios.runtime.world.concepts.IsSystemLike;

import helios.runtime.concepts.HasTag;
import helios.runtime.world.tags.SystemRole;

import helios.runtime.world.UpdateContext;

using namespace helios::runtime::concepts;
export namespace helios::runtime::world::concepts {

    /**
     * @brief Constrains T to types eligible for System registration.
     *
     * @details A type satisfies IsSystemLike if it declares the system role tag
     * and exposes one of the following update signatures:
     * 1. `void update(UpdateContext&)`
     * 2. `void update(UpdateContext&, T::CommandBuffer_type&)`
     *
     * The second variant is used for systems requiring a typed command buffer
     * injected by the world/gameloop registration path.
     *
     * @tparam T The type to constrain.
     *
     * @see SystemRole
     * @see HasTag
     * @see System
     */
    template<class T>
    concept IsSystemLike = (requires(T& t, helios::runtime::world::UpdateContext& updateContext) {
        {t.update(updateContext) } -> std::same_as<void>;
    } || requires(T& t, helios::runtime::world::UpdateContext& updateContext, typename T::CommandBuffer_type& buf)
    {
        t.update(updateContext, buf);
    }) && HasTag<T, tags::SystemRole>;
}
