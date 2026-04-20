/**
 * @file DamageOnCollisionSystem.ixx
 * @brief System that applies damage when solid collisions occur.
 */
module;

#include <format>

#include <string>

export module helios.gameplay.damage.systems.DamageOnCollisionSystem;


import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.state.Bindings;


import helios.physics.collision.events.SolidCollisionEvent;

import helios.gameplay.damage.components.DamageDealerComponent;
import helios.gameplay.common.types;

import helios.gameplay.spawn.components.EmittedByComponent;

import helios.gameplay.damage.components.LastDamageComponent;
import helios.gameplay.damage.commands.ApplyDamageCommand;

import helios.gameplay.health.components.HealthComponent;
import helios.gameplay.health.types;
import helios.gameplay.health.events;

import helios.math;
import helios.util;

import helios.runtime.world.tags.SystemRole;

using namespace helios::gameplay::health::types;
using namespace helios::gameplay::health::events;
using namespace helios::gameplay::health::components;
using namespace helios::gameplay::damage::components;
using namespace helios::gameplay::damage::commands;
using namespace helios::gameplay::spawn::components;

using namespace helios::gameplay::common::types;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::messaging::command::concepts;

#define HELIOS_LOG_SCOPE "helios::gameplay::damage::systems::DamageOnCollisionSystem"
export namespace helios::gameplay::damage::systems {


    /**
     * @brief Emits HealthChangedEvents when solid collisions involve damage dealers.
     *
     * Listens for SolidCollisionEvents and checks if the source entity has a
     * DamageDealerComponent. If the target has a HealthComponent, emits an
     * ApplyDamageCommand. Resolves the true attacker (instigator) via
     * EmittedByComponent when applicable (e.g. projectiles).
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class DamageOnCollisionSystem {


        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;
        using CommandBuffer_type = TCommandBuffer;


        /**
         * @brief Processes collision events and applies damage to targets.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            auto eventPass = updateContext.readPass<
                helios::physics::collision::events::SolidCollisionEvent<THandle>>();

            for (const auto& event : eventPass) {

                auto go = updateContext.find(event.collisionContext().source);

                if (!go) {
                    continue;
                }

                auto* ddc = go->get<DamageDealerComponent>();
                if (!ddc) {
                    continue;
                }

                auto other = event.collisionContext().other;
                if (!other.has_value()) {
                    continue;
                }

                auto target = updateContext.find(other.value());
                if (!target) {
                    continue;
                }
                auto* hc = target->get<HealthComponent>();
                if (!hc) {
                    continue;
                }


                const auto damageApplied = ddc->damage(event.collisionContext().otherCollisionLayerId);

                logger_.info(std::format(
                    "Hitting entity {0} with {1} damage!", other.value().entityId, damageApplied
                ));

                // the go itself is the source
                auto instigator= go->handle();
                auto causer = go->handle();

                auto* ebc = go->get<EmittedByComponent>();
                if (ebc) {
                    // else the source is assigned to the go emitted
                    // by the go
                    instigator = ebc->source();
                }

                const auto interactionContext = InteractionContext{
                    .target = other.value(),
                    .instigator = instigator,
                    .causer = causer,
                    .contact = event.collisionContext().contact,
                };

                auto dc = DamageContext{
                    .interactionContext = interactionContext,
                    .damage = damageApplied
                };

                cmdBuffer.template add<ApplyDamageCommand>(dc);

            }

        }

    };


}