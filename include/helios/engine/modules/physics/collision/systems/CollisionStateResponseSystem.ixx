/**
 * @file CollisionStateResponseSystem.ixx
 * @brief System for processing collision states and executing collision behaviors.
 */
module;

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <helios/helios_config.h>
#include <unordered_set>
#include <vector>


export module helios.engine.modules.physics.collision.systems.CollisionStateResponseSystem;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;


import helios.state.Bindings;


import helios.engine.modules.physics.collision.types.CollisionBehavior;

import helios.engine.modules.physics.collision.components.CollisionStateComponent;

import helios.gameplay.spawn.commands.DespawnCommand;
import helios.engine.modules.physics.collision.events;

import helios.gameplay.spawn.components.SpawnedByProfileComponent;

import helios.ecs.components.Active;

using namespace helios::engine::modules::physics::collision::components;
using namespace helios::engine::modules::physics::collision::types;
using namespace helios::gameplay::spawn::commands;
using namespace helios::runtime::messaging::command;
using namespace helios::engine::common::concepts;


import helios.engine.common.tags.SystemRole;

export namespace helios::engine::modules::physics::collision::systems {

    /**
     * @brief System that processes collision states and executes configured behaviors.
     *
     * @details This system reads the CollisionStateComponent of each entity to determine
     * how to respond to detected collisions. Based on the configured CollisionBehavior,
     * the system issues appropriate commands (e.g., DespawnCommand for entities marked
     * with Despawn behavior).
     *
     * The system requires entities to have both a CollisionStateComponent (for collision
     * data) and a SpawnedByProfileComponent.
     *
     * Supported behaviors:
     * - **Despawn:** Issues a DespawnCommand to return the entity to its object pool.
     *
     * This system should run after collision detection but before collision state clearing.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
     class CollisionStateResponseSystem {

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Processes collision states and issues response commands.
         *
         * @details Iterates over all entities with CollisionStateComponent and PoolIdComponent.
         * For entities with active collisions, reads the collision behavior and issues
         * appropriate commands and/or events.
         *
         * @param updateContext Context providing access to the command buffer and world.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, csc, sbp, active] : updateContext.view<
                THandle,
                CollisionStateComponent<THandle>,
                helios::gameplay::spawn::components::SpawnedByProfileComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                if (!csc->hasCollision()) {
                    continue;
                }

                CollisionBehavior collisionBehavior = csc->collisionBehavior();

                if (hasFlag(collisionBehavior, CollisionBehavior::PassEvent)) {

                    if (csc->isTrigger()) {
                        updateContext.pushPass<events::TriggerCollisionEvent<THandle>>(
                            entity.handle(),
                            csc->collisionContext()
                       );
                    }

                    if (csc->isSolid()) {
                        updateContext.pushPass<events::SolidCollisionEvent<THandle>>(
                            entity.handle(),
                            csc->collisionContext()
                       );
                    }
                }


                if (hasFlag(collisionBehavior, CollisionBehavior::Despawn)) {
                    updateContext.queueCommand<TCommandBuffer, DespawnCommand<THandle>>(
                        entity.handle(), sbp->spawnProfileId());
                }
            }

        }

    };


}
