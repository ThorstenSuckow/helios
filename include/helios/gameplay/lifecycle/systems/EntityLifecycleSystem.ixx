/**
 * @file EntityLifecycleSystem.ixx
 * @brief System that processes health depletion events and triggers despawning.
 */
module;


#include <vector>
#include <cassert>

export module helios.gameplay.lifecycle.systems.EntityLifecycleSystem;

import helios.state.Bindings;




import helios.runtime.world.Manager;
import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.gameplay.spawn;

import helios.gameplay.health.events;
import helios.gameplay.health.types;

import helios.gameplay.matchstate.components;
import helios.gameplay.matchstate.events;

import helios.gameplay.health;

import helios.gameplay.spawn.components.SpawnedByProfileComponent;

using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::messaging::command::concepts;

using namespace helios::gameplay::matchstate::components;
using namespace helios::gameplay::matchstate::events;
using namespace helios::gameplay::health::types;
using namespace helios::gameplay::health::events;
using namespace helios::gameplay::spawn::components;
using namespace helios::gameplay::spawn::commands;

import helios.runtime.world.tags.SystemRole;

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
     class EntityLifecycleSystem {

    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;
        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Processes health depletion events and enqueues despawn commands.
         *
         * @param updateContext Current frame context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            auto events = updateContext.readPass<HealthDepletedEvent<THandle>>();

            for (auto& event : events) {
                auto go = updateContext.find(event.source());

                if (go) {

                    auto* hc = go->get<helios::gameplay::health::components::HealthComponent>();
                    if (hc) {
                        auto healthDepletedBehavior = hc->healthDepletedBehavior();
                        if (hasHealthDepletedFlag(healthDepletedBehavior, HealthDepletedBehavior::Despawn)) {
                            if (auto* sbp = go->template get<SpawnedByProfileComponent>()) {
                                assert(sbp->spawnProfileId().value() != 0 && "Entity has no SpawnProfileId.");
                                cmdBuffer.template add<DespawnCommand<THandle>>(go->handle(), sbp->spawnProfileId());
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