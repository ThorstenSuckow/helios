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


export module helios.engine.modules.physics.collision.systems.CollisionStateClearSystem;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.ecs.System;

import helios.engine.runtime.messaging.command.CommandBuffer;

import helios.engine.modules.physics.collision.types.CollisionBehavior;

import helios.engine.modules.physics.collision.components.CollisionStateComponent;

import helios.engine.runtime.spawn.commands.DespawnCommand;

import helios.engine.mechanics.lifecycle.components.Active;

using namespace helios::engine::modules::physics::collision::components;
using namespace helios::engine::modules::physics::collision::types;
using namespace helios::engine::runtime::spawn::commands;



export namespace helios::engine::modules::physics::collision::systems {

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
    class CollisionStateClearSystem : public helios::engine::ecs::System {

    public:

        /**
         * @brief Resets collision state for all entities with CollisionStateComponent.
         *
         * @param updateContext Context containing frame data and world access.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {
            for (auto [entity, csc, active] : gameWorld_->view<
               CollisionStateComponent,
               helios::engine::mechanics::lifecycle::components::Active
           >().whereEnabled()) {
                csc->reset();
           }
        }

    };


}
