/**
 * @file DamageOnCollisionSystem.ixx
 * @brief System that applies damage when solid collisions occur.
 */
module;

#include <format>
#include <helios/engine/ecs/query/GameObjectView.h>
#include <string>

export module helios.engine.mechanics.damage.systems.DamageOnCollisionSystem;

import helios.engine.ecs.System;
import helios.engine.ecs.GameObject;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

import helios.engine.modules.physics.collision.events.SolidCollisionEvent;

import helios.engine.mechanics.damage.components.DamageDealerComponent;

import helios.engine.mechanics.spawn.components.EmittedByComponent;

import helios.engine.mechanics.combat.types.AttackContext;
import helios.engine.mechanics.combat.components.LastAttackerComponent;

import helios.engine.mechanics.health.components.HealthComponent;

import helios.math;
import helios.util;

#define HELIOS_LOG_SCOPE "helios::engine::mechanics::damage::systems::DamageOnCollisionSystem"
export namespace helios::engine::mechanics::damage::systems {


    /**
     * @brief System that processes solid collision events and applies damage.
     *
     * Listens for SolidCollisionEvent and checks if the source entity has a
     * DamageDealerComponent. If the target has a HealthComponent, applies the
     * appropriate damage based on the target's collision layer.
     *
     * Also updates the target's LastAttackerComponent with the attack context
     * for attribution in death events and scoring.
     */
    class DamageOnCollisionSystem : public helios::engine::ecs::System {


        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

    public:


        /**
         * @brief Processes collision events and applies damage to targets.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            auto eventPass = updateContext.readPass<
                helios::engine::modules::physics::collision::events::SolidCollisionEvent>();

            for (const auto& event : eventPass) {

                auto* go = updateContext.gameWorld().find(event.collisionContext().source);

                if (!go) {
                    continue;
                }

                auto* ddc = go->get<helios::engine::mechanics::damage::components::DamageDealerComponent>();
                if (!ddc) {
                    continue;
                }

                auto other = event.collisionContext().other;
                if (!other.has_value()) {
                    continue;
                }
                const auto* target = updateContext.gameWorld().find(other.value());
                if (!target) {
                    continue;
                }
                auto* hc = target->get<helios::engine::mechanics::health::components::HealthComponent>();
                if (!hc) {
                    continue;
                }


                const auto damageApplied = ddc->damage(event.collisionContext().otherCollisionLayerId);
                hc->takeDamage(damageApplied);
                logger_.info(std::format(
                    "Hitting entity {0} with {1} damage! {2} health left ",
                    other.value().value(),
                    damageApplied,
                    hc->health()
                ));

                auto hitman = go->guid();

                auto* ebc = go->get<helios::engine::mechanics::spawn::components::EmittedByComponent>();
                if (ebc) {
                    hitman = ebc->source();
                }

                auto* lac = target->get<helios::engine::mechanics::combat::components::LastAttackerComponent>();
                if (lac) {
                    auto attackContext = helios::engine::mechanics::combat::types::AttackContext{
                        .source = hitman,
                        .contact = event.collisionContext().contact,
                        .damageApplied = damageApplied
                    };
                    lac->setLastAttackContext(attackContext);
                }

            }

        }

    };


}