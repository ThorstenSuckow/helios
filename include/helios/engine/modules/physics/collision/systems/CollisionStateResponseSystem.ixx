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

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.ecs.System;

import helios.engine.runtime.messaging.command.CommandBuffer;

import helios.engine.modules.physics.collision.types.CollisionBehavior;

import helios.engine.modules.physics.collision.components.CollisionStateComponent;

import helios.engine.runtime.spawn.commands.DespawnCommand;
import helios.engine.modules.physics.collision.events;

import helios.engine.mechanics.spawn.components.SpawnedByProfileComponent;

import helios.engine.mechanics.lifecycle.components.Active;

using namespace helios::engine::modules::physics::collision::components;
using namespace helios::engine::modules::physics::collision::types;
using namespace helios::engine::runtime::spawn::commands;


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
    class CollisionStateResponseSystem : public helios::engine::ecs::System {

    public:

        /**
         * @brief Processes collision states and issues response commands.
         *
         * @details Iterates over all entities with CollisionStateComponent and PoolIdComponent.
         * For entities with active collisions, reads the collision behavior and issues
         * appropriate commands and/or events.
         *
         * @param updateContext Context providing access to the command buffer and world.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, csc, sbp, active] : gameWorld_->view<
                CollisionStateComponent,
                helios::engine::mechanics::spawn::components::SpawnedByProfileComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                if (!csc->hasCollision()) {
                    continue;
                }

                CollisionBehavior collisionBehavior = csc->collisionBehavior();

                if (hasFlag(collisionBehavior, CollisionBehavior::PassEvent)) {

                    if (csc->isTrigger()) {
                        updateContext.pushPass<events::TriggerCollisionEvent>(
                            entity.entityHandle(),
                            csc->collisionContext()
                       );
                    }

                    if (csc->isSolid()) {
                        updateContext.pushPass<events::SolidCollisionEvent>(
                            entity.entityHandle(),
                            csc->collisionContext()
                       );
                    }
                }


                if (hasFlag(collisionBehavior, CollisionBehavior::Despawn)) {
                    updateContext.commandBuffer().add<DespawnCommand>(
                        entity.entityHandle(), sbp->spawnProfileId());
                }
            }

        }

    };


}
