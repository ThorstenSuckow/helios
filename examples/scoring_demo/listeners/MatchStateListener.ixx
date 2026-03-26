/**
 * @file MatchStateListener.ixx
 * @brief Match state listener installation for the scoring demo.
 */
module;

#include <cassert>
#include <cmath>
#include <memory>

export module helios.examples.scoring.MatchStateListener;

import helios;
import helios.examples.scoring.IdConfig;
import helios.examples.scoring.ArenaConfig;

using namespace helios::engine::state;
using namespace helios::engine::state::types;
using namespace helios::engine::state::listeners;
using namespace helios::engine::state::commands;
using namespace helios::engine::runtime::world;
using namespace helios::engine::mechanics::match::types;
using namespace helios::engine::mechanics::match::components;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::timing;
using namespace helios::engine::mechanics::timing::types;
using namespace helios::engine::mechanics::lifecycle::components;
using namespace helios::engine::mechanics::health::components;
using namespace helios::engine::modules::spatial::transform::components;
using namespace helios::engine::modules::physics::motion::components;
using namespace helios::engine::modules::physics::collision::components;
using namespace helios::engine::ecs::types;

export namespace helios::examples::scoring {

    /**
     * @brief Installs match state transition listeners.
     *
     * @param matchStateManager The match state manager to register listeners with.
     */
    inline void installMatchStateListeners(
        StateManager<MatchState>& matchStateManager, TimerManager& timerManager
    ) {
        matchStateManager.addStateListener(
            std::make_unique<LambdaStateListener<MatchState>>(
            [](UpdateContext& updateContext, const MatchState from) -> void {



            },
            [](UpdateContext& updateContext,
                 const StateTransitionContext<MatchState> transitionContext) -> void {

            },
            [&timerManager](UpdateContext& updateContext, const MatchState to) -> void {

                auto player = updateContext.find(updateContext.session().playerEntityHandle());

                assert(player && "Unexpected missing player object");


                if (to == MatchState::Start) {
                    player->setActive(true);
                    player->get<LivesComponent>()->reset();
                }

                if (to == MatchState::Playing) {
                    player->getOrAdd<DelayedComponentEnabler>().defer(
                        *player, ComponentTypeId::id<CollisionComponent>(), 3.0f
                    );
                }

                if (to == MatchState::Countdown) {
                    auto* tsc = player->get<TranslationStateComponent>();
                    tsc->setTranslation(ArenaConfig::SPACESHIP_SPAWN_POSITION);

                    auto* sc = player->get<SteeringComponent>();
                    auto* mc = player->get<Move2DComponent>();
                    sc->reset();
                    mc->reset();

                    player->get<HealthComponent>()->reset();

                    player->remove<DeadTagComponent>();
                }

                if (to == MatchState::PlayerDefeated) {

                    // reuse the respawntimer for both cases
                    timerManager.gameTimer(IdConfig::PlayerRespawnTimerId)->reset(TimerState::Running);

                    auto* lc = player->get<LivesComponent>();
                    if (lc->lives() == 0) {
                        updateContext.queueCommand<DelayedStateCommand<MatchState>>(
                            StateTransitionRequest<MatchState>(
                            MatchState::PlayerDefeated,
                                MatchStateTransitionId::GameOverRequest
                            ), IdConfig::PlayerRespawnTimerId
                        );

                        return;
                    }

                    player->get<CollisionComponent>()->disable();
                    updateContext.queueCommand<DelayedStateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(
                            MatchState::PlayerDefeated,
                            MatchStateTransitionId::CountdownRequest
                        ), IdConfig::PlayerRespawnTimerId
                    );
                }

                if (to == MatchState::GameOver || to == MatchState::Warmup) {
                    player->setActive(false);
                }


            })
        );
    }

}