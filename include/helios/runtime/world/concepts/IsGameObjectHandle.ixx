/**
 * @file IsGameObjectHandle.ixx
 * @brief Concept that matches the runtime game-object handle type.
 */
module;

#include <concepts>

export module helios.runtime.world.concepts.IsGameObjectHandle;

import helios.runtime.world.types.GameObjectHandle;

using namespace helios::runtime::world::types;
export namespace helios::runtime::world::concepts {

    /**
     * @brief Constrains a type to `GameObjectHandle`.
     *
     * @tparam THandle Candidate handle type.
     */
    template<typename THandle>
    concept IsGameObjectHandle =
        std::is_same_v<THandle, GameObjectHandle>;


}