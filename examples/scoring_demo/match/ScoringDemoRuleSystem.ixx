/**
 * @file ScoringDemoRuleSystem.ixx
 * @brief Match-rule system for the scoring demo.
 */
module;


#include <vector>
#include <cassert>

export module helios.examples.scoring.ScoringDemoRuleSystem;

import helios.engine.state.Bindings;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.ecs;

import helios.engine.state.Bindings;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.mechanics.match;
import helios.engine.mechanics.gamestate;

import helios.engine.state;


import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.spawn;

import helios.engine.mechanics.health.events;
import helios.engine.mechanics.health.types;

import helios.engine.mechanics.match.components;
import helios.engine.mechanics.match.events;
import helios.engine.mechanics.lifecycle.components;

using namespace helios::engine::ecs;

using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;

using namespace helios::engine::mechanics::lifecycle::components;
using namespace helios::engine::mechanics::match::components;
using namespace helios::engine::mechanics::match::events;
using namespace helios::engine::mechanics::health::types;
using namespace helios::engine::mechanics::health::events;
using namespace helios::engine::runtime::spawn::commands;
using namespace helios::engine::runtime::world;
using namespace helios::engine::state::commands;
using namespace helios::engine::state::types;
using namespace helios::engine::mechanics::match::types;


export namespace helios::examples::scoring {

    /**
     * @brief Processes player death events and decrements lives.
     *
     * Reads HealthDepletedEvents from the phase bus and, for the player
     * entity, decreases the remaining lives via LivesComponent.
     */
    class ScoringDemoRuleSystem : public helios::engine::ecs::System {


        /**
         * @brief Handles death for a single entity by decrementing lives.
         *
         * @param go The deceased GameObject.
         * @param updateContext Current frame context.
         */
        bool onDeath(GameObject go, UpdateContext& updateContext) noexcept {
            if (auto* lc = go.get<LivesComponent>()) {
                if (lc->lives() > 0) {
                    lc->decrease();
                    return true;
                    updateContext.commandBuffer().add<StateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(
                            MatchState::Playing, MatchStateTransitionId::PlayerDied
                        )
                    );
                }
            }
            return false;
        }

    public:

        /**
         * @brief Reads health depletion events and triggers life loss for the player.
         *
         * @param updateContext Current frame context.
         */
        void update(UpdateContext& updateContext) noexcept override {


            for (auto events = updateContext.readPass<HealthDepletedEvent>(); auto& event : events) {
                if (auto go = updateContext.find(event.source())) {
                    if (onDeath(*go, updateContext)) {
                        return;
                    }

                }
            }

        }
    };

}

