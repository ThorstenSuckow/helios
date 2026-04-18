/**
 * @file GameObjectLifecycleSystem.ixx
 * @brief System that processes health depletion events and triggers despawning.
 */
module;


#include <vector>
#include <cassert>

export module helios.engine.mechanics.lifecycle.systems.GameObjectLifecycleSystem;

import helios.engine.state.Bindings;




import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.engine.runtime.spawn;

import helios.engine.mechanics.health.events;
import helios.engine.mechanics.health.types;

import helios.engine.mechanics.match.components;
import helios.engine.mechanics.match.events;

import helios.engine.mechanics.health;

import helios.engine.mechanics.spawn.components.SpawnedByProfileComponent;

using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::common::concepts;

using namespace helios::engine::mechanics::match::components;
using namespace helios::engine::mechanics::match::events;
using namespace helios::engine::mechanics::health::types;
using namespace helios::engine::mechanics::health::events;
using namespace helios::engine::mechanics::spawn::components;
using namespace helios::engine::runtime::spawn::commands;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::mechanics::lifecycle::systems {

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
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            auto events = updateContext.readPass<HealthDepletedEvent<THandle>>();

            for (auto& event : events) {
                auto go = updateContext.find(event.source());

                if (go) {

                    auto* hc = go->get<helios::engine::mechanics::health::components::HealthComponent>();
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