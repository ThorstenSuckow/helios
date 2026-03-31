/**
 * @file ScoringDemoRuleSystem.ixx
 * @brief Match-rule system for the scoring demo.
 */
module;


#include <vector>
#include <cassert>

export module helios.examples.scoring.ScoringDemoRuleSystem;

import helios.examples.scoring.IdConfig;

import helios.engine.state.Bindings;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.ecs;

import helios.engine.state.Bindings;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.mechanics.match;
import helios.engine.mechanics.gamestate;

import helios.engine.state;


import helios.engine.runtime.world;
import helios.engine.runtime.spawn;

import helios.engine.mechanics.health.events;
import helios.engine.mechanics.health.types;

import helios.engine.mechanics.timing.TimerManager;
import helios.engine.mechanics.timing.types;

import helios.engine.mechanics.match.components;
import helios.engine.mechanics.match.events;
import helios.engine.mechanics.lifecycle.components;

import helios.engine.common.tags;

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
using namespace helios::engine::mechanics::timing;
using namespace helios::engine::mechanics::timing::types;


export namespace helios::examples::scoring {

    /**
     * @brief Processes player death events and decrements lives.
     *
     * Reads HealthDepletedEvents from the phase bus and, for the player
     * entity, decreases the remaining lives via LivesComponent.
     */
    class ScoringDemoRuleSystem  {


        /**
         * @brief Handles death for a single entity by decrementing lives.
         *
         * @details If the deceased entity is the player, also triggers a state
         * transition to handle respawn or game over logic.
         *
         * @param go The deceased GameObject.
         * @param updateContext Current frame context.
         */
        void onDeath(GameObject go, UpdateContext& updateContext) noexcept {
            if (auto* lc = go.get<LivesComponent>()) {
                lc->decrease();
            }

            if (updateContext.session().playerEntityHandle() == go.entityHandle()) {
                updateContext.queueCommand<StateCommand<MatchState>>(
                    StateTransitionRequest<MatchState>(
                        MatchState::Playing, MatchStateTransitionId::PlayerDied
                    )
                );
            }
        }


        TimerManager& timerManager_;

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        explicit ScoringDemoRuleSystem(TimerManager& timerManager)
                : timerManager_(timerManager) {}

        /**
         * @brief Reads health depletion events and triggers life loss for the player.
         *
         * @param updateContext Current frame context.
         */
        void update(UpdateContext& updateContext) noexcept {

            if (timerManager_.gameTimer(IdConfig::DemoTimerId)->isFinished()) {
                // timer is up, switch to game over state
                // nothing fancy here.
                updateContext.queueCommand<StateCommand<MatchState>>(
                    StateTransitionRequest<MatchState>(
                        MatchState::Playing, MatchStateTransitionId::GameOverRequest
                    )
                );
                return;
            }

            for (auto events = updateContext.readPass<HealthDepletedEvent>(); auto& event : events) {
                if (auto go = updateContext.find(event.source())) {
                    onDeath(*go, updateContext);
                }
            }

        }
    };

}

