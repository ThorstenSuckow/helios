/**
 * @file HealthManager.ixx
 * @brief Manager for processing damage commands and emitting health events.
 */
module;

#include <vector>

export module helios.engine.mechanics.health.HealthManager;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.Manager;
import helios.engine.runtime.messaging.command.TypedCommandHandler;

import helios.engine.mechanics.damage.commands.ApplyDamageCommand;
import helios.engine.common.types;
import helios.engine.mechanics.damage.components.LastDamageComponent;

import helios.engine.mechanics.health.components.HealthComponent;
import helios.engine.mechanics.health.types;
import helios.engine.mechanics.health.events;

import helios.engine.mechanics.lifecycle.components;

import helios.util.log;

using namespace helios::engine::mechanics::health::types;
using namespace helios::engine::mechanics::health::components;
using namespace helios::engine::mechanics::health::events;
using namespace helios::engine::mechanics::lifecycle::components;
using namespace helios::engine::common::types;
using namespace helios::engine::mechanics::damage::components;
using namespace helios::engine::mechanics::damage::commands;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::runtime::world;

#define HELIOS_LOG_SCOPE "helios::engine::mechanics::health::HealthManager"
export namespace helios::engine::mechanics::health {

    /**
     * @brief Manager that processes damage commands and updates entity health.
     *
     * HealthManager implements the Manager interface for integration into the
     * game loop flush cycle, and the TypedCommandHandler interface for
     * ApplyDamageCommand so that incoming damage is queued and applied in batch.
     *
     * During flush(), each queued DamageContext is resolved: the target's
     * HealthComponent is damaged, a HealthChangedEvent is emitted, and if
     * health reaches zero a HealthDepletedEvent is pushed together with an
     * optional DeadTagComponent.
     *
     * Event phases:
     * - HealthChangedEvent is pushed to the **phase** bus
     *   (available to systems in the next phase).
     * - HealthDepletedEvent is pushed to the **pass** bus
     *   (available to systems after commit within the same pass).
     *
     * @see ApplyDamageCommand
     * @see HealthComponent
     * @see HealthDepletedEvent
     * @see Manager
     */
    class HealthManager : public Manager, public TypedCommandHandler<ApplyDamageCommand> {

        /**
         * @brief Logger scoped to HealthManager.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

        /**
         * @brief Pending damage contexts collected via submit().
         */
        std::vector<DamageContext> damageContexts_;

    public:

        /**
         * @brief Applies all queued damage and emits health events.
         *
         * For each pending DamageContext the target's HealthComponent is
         * looked up and damaged. A HealthChangedEvent is pushed per hit.
         * When health transitions from alive to dead, the configured
         * HealthDepletedBehavior flags are evaluated and a
         * HealthDepletedEvent is emitted.
         *
         * HealthChangedEvent is pushed to the phase bus (available next phase);
         * HealthDepletedEvent is pushed to the pass bus (available after
         * commit within the same pass).
         *
         * @param updateContext The current frame's update context.
         */
        void flush(UpdateContext& updateContext) noexcept override {

            for (const auto& damageContext : damageContexts_) {

                auto interactionContext = damageContext.interactionContext;

                auto target = updateContext.find(interactionContext.target);

                auto* hc = target->get<HealthComponent>();
                if (!hc) {
                    continue;
                }

                bool wasAlive = hc->isAlive();
                auto damageApplied = damageContext.damage;
                hc->takeDamage(damageApplied);

                auto* lac = target->get<LastDamageComponent>();
                if (lac && damageApplied > 0) {
                    lac->setDamageContext(damageContext);
                }

                auto huc = HealthChangeContext{
                    interactionContext,
                    -damageApplied
                };

                updateContext.pushPhase<HealthChangedEvent>(huc);

                if (wasAlive && !hc->isAlive()) {
                    auto healthDepletedBehavior = hc->healthDepletedBehavior();

                    if (hasHealthDepletedFlag(healthDepletedBehavior, HealthDepletedBehavior::DeadTag)) {
                        target->add<DeadTagComponent>();
                    }

                    updateContext.pushPass<HealthDepletedEvent>(target->entityHandle(), damageContext);
                }
            }

            damageContexts_.clear();
        }

        /**
         * @brief Enqueues a damage command for deferred processing.
         *
         * @param applyDamageCommand The damage command to enqueue.
         *
         * @return True if the command was accepted.
         */
        bool submit(
            const ApplyDamageCommand applyDamageCommand
        ) noexcept override {
            damageContexts_.push_back(applyDamageCommand.damageContext());
            return true;
        };

        /**
         * @brief Registers this manager as the damage command handler.
         *
         * @param gameWorld The game world to register with.
         */
        void init(GameWorld& gameWorld) override {
            gameWorld.template registerCommandHandler<TypedCommandHandler<ApplyDamageCommand>>(*this);
        }

        /**
         * @brief Clears all pending damage contexts.
         */
        void reset() override {
            damageContexts_.clear();
        }

    };

}
