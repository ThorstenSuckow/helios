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


export module helios.engine.game.physics.collision.systems.CollisionStateResponseSystem;

import helios.engine.ecs.GameWorld;
import helios.engine.ecs.UpdateContext;
import helios.engine.ecs.System;

import helios.engine.core.messaging.command.CommandBuffer;

import helios.engine.game.physics.collision.types.CollisionBehavior;

import helios.engine.game.physics.collision.components.CollisionStateComponent;

import helios.engine.mechanics.spawn.commands.DespawnCommand;

import helios.engine.game.pool.components.PoolIdComponent;

using namespace helios::engine::game::physics::collision::components;
using namespace helios::engine::game::physics::collision::types;
using namespace helios::engine::mechanics::spawn::commands;


export namespace helios::engine::game::physics::collision::systems {

    /**
     * @brief System that processes collision states and executes configured behaviors.
     *
     * @details This system reads the CollisionStateComponent of each entity to determine
     * how to respond to detected collisions. Based on the configured CollisionBehavior,
     * the system issues appropriate commands (e.g., DespawnCommand for entities marked
     * with Despawn behavior).
     *
     * The system requires entities to have both a CollisionStateComponent (for collision
     * data) and a PoolIdComponent (for pool-based lifecycle management).
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
         * appropriate commands to the command buffer.
         *
         * @param updateContext Context providing access to the command buffer and world.
         */
        void update(helios::engine::ecs::UpdateContext& updateContext) noexcept override {

            for (auto [entity, csc, pic] : gameWorld_->find<
                CollisionStateComponent,
                helios::engine::game::pool::components::PoolIdComponent
            >().each()) {

                if (!csc->hasCollision()) {
                    continue;
                }

                CollisionBehavior collisionBehavior = csc->collisionBehavior();

                if (collisionBehavior == CollisionBehavior::Despawn) {
                    updateContext.commandBuffer().add<DespawnCommand>(
                        entity->guid(),
                        pic->gameObjectPoolId()
                    );
                }

            }

        }

    };


}
