/**
 * @file GameObjectId.ixx
 * @brief Strong id type for runtime game objects.
 */
module;

export module helios.runtime.world.types.GameObjectId;

import helios.core.types.StrongId;

using namespace helios::core::types;
export namespace helios::runtime::world::types {

    /**
     * @brief Tag type used to scope `GameObjectId`.
     */
    struct GameObjectTag{};

    /**
     * @brief Strong id used for game-object handle identity.
     */
    using GameObjectId = StrongId<GameObjectTag>;

}