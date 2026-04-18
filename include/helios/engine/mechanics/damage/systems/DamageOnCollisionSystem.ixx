/**
 * @file DamageOnCollisionSystem.ixx
 * @brief System that applies damage when solid collisions occur.
 */
module;

#include <format>

#include <string>

export module helios.engine.mechanics.damage.systems.DamageOnCollisionSystem;


import helios.engine.runtime.world.GameObject;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.engine.state.Bindings;


import helios.engine.modules.physics.collision.events.SolidCollisionEvent;

import helios.engine.mechanics.damage.components.DamageDealerComponent;
import helios.engine.common.types;

import helios.engine.mechanics.spawn.components.EmittedByComponent;

import helios.engine.mechanics.damage.components.LastDamageComponent;
import helios.engine.mechanics.damage.commands.ApplyDamageCommand;

import helios.engine.mechanics.health.components.HealthComponent;
import helios.engine.mechanics.health.types;
import helios.engine.mechanics.health.events;

import helios.math;
import helios.util;

import helios.engine.common.tags.SystemRole;

using namespace helios::engine::mechanics::health::types;
using namespace helios::engine::mechanics::health::events;
using namespace helios::engine::mechanics::health::components;
using namespace helios::engine::mechanics::damage::components;
using namespace helios::engine::mechanics::damage::commands;
using namespace helios::engine::mechanics::spawn::components;

using namespace helios::engine::common::types;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::common::concepts;

#define HELIOS_LOG_SCOPE "helios::engine::mechanics::damage::systems::DamageOnCollisionSystem"
export namespace helios::engine::mechanics::damage::systems {


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

        using EngineRoleTag = helios::engine::common::tags::SystemRole;


        /**
         * @brief Processes collision events and applies damage to targets.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            auto eventPass = updateContext.readPass<
                helios::engine::modules::physics::collision::events::SolidCollisionEvent<THandle>>();

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

                updateContext.queueCommand<TCommandBuffer, ApplyDamageCommand>(dc);

            }

        }

    };


}