/**
 * @file CollisionStateClearSystem.ixx
 * @brief System for resetting collision state components each frame.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <helios/helios_config.h>
#include <unordered_set>
#include <vector>


export module helios.physics.collision.systems.CollisionStateClearSystem;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;



import helios.physics.collision.types.CollisionBehavior;

import helios.physics.collision.components.CollisionStateComponent;

import helios.gameplay.spawn.commands.DespawnCommand;

import helios.ecs.components.Active;

using namespace helios::physics::collision::components;
using namespace helios::physics::collision::types;
using namespace helios::gameplay::spawn::commands;



import helios.runtime.world.tags.SystemRole;

export namespace helios::physics::collision::systems {

    /**
     * @brief System that clears collision state components at the end of each frame.
     *
     * @details This system iterates over all entities with a CollisionStateComponent
     * and resets their collision state. It should run in the post-phase of the game loop
     * to ensure collision data from the current frame does not persist into the next frame.
     *
     * Running this system after collision response systems have processed their events
     * ensures a clean slate for the next collision detection pass.
     */
    template<typename THandle>
    class CollisionStateClearSystem {

    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;
        /**
         * @brief Resets collision state for all entities with CollisionStateComponent.
         *
         * @param updateContext Context containing frame data and world access.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {
            for (auto [entity, csc, active] : updateContext.view<
               THandle,
               CollisionStateComponent<THandle>,
               helios::ecs::components::Active<THandle>
           >().whereEnabled()) {
                csc->reset();
           }
        }

    };


}
