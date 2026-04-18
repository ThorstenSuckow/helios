/**
 * @file GameObjectLifecycleSystem.ixx
 * @brief System that processes health depletion events and triggers despawning.
 */
module;


#include <vector>
#include <cassert>

export module helios.gameplay.lifecycle.systems.GameObjectLifecycleSystem;

import helios.state.Bindings;




import helios.runtime.world.Manager;
import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.runtime.spawn;

import helios.gameplay.health.events;
import helios.gameplay.health.types;

import helios.gameplay.matchstate.components;
import helios.gameplay.matchstate.events;

import helios.gameplay.health;

import helios.gameplay.spawn.components.SpawnedByProfileComponent;

using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::engine::common::concepts;

using namespace helios::gameplay::matchstate::components;
using namespace helios::gameplay::matchstate::events;
using namespace helios::gameplay::health::types;
using namespace helios::gameplay::health::events;
using namespace helios::gameplay::spawn::components;
using namespace helios::runtime::spawn::commands;

import helios.engine.common.tags.SystemRole;

export namespace helios::gameplay::lifecycle::systems {

    /**
     * @brief Reacts to HealthDepletedEvents and despawns or deactivates entities.
     *
     * Reads HealthDepletedEvents from the phase bus. For each affected entity,
     * the system inspects the HealthComponent's HealthDepletedBehavior flags:
     * - If Despawn is set and the entity has a SpawnedByProfileComponent,
     *   a DespawnCommand is enqueued.
     * - Otherwise the entity is deactivated.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
     class GameObjectLifecycleSystem {

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Processes health depletion events and enqueues despawn commands.
         *
         * @param updateContext Current frame context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            auto events = updateContext.readPass<HealthDepletedEvent<THandle>>();

            for (auto& event : events) {
                auto go = updateContext.find(event.source());

                if (go) {

                    auto* hc = go->get<helios::gameplay::health::components::HealthComponent>();
                    if (hc) {
                        auto healthDepletedBehavior = hc->healthDepletedBehavior();
                        if (hasHealthDepletedFlag(healthDepletedBehavior, HealthDepletedBehavior::Despawn)) {
                            if (auto* sbp = go->get<SpawnedByProfileComponent>()) {
                                assert(sbp->spawnProfileId().value() != 0 && "Entity has no SpawnProfileId.");
                                updateContext.queueCommand<TCommandBuffer, DespawnCommand<THandle>>(go->handle(), sbp->spawnProfileId());
                            } else {
                                go->setActive(false);
                                /**
                                 * @todo remove from gameworld
                                 */
                            }
                        }
                    }
                }

            }

        }
    };

}