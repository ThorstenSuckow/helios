/**
 * @file MatchStateManager.ixx
 * @brief Match state manager and transition orchestrator.
 */
module;

#include <cassert>
#include <memory>
#include <stdexcept>
#include <unordered_map>

export module helios.engine.mechanics.match.MatchStateManager;


import helios.engine.mechanics.match.MatchStateCommandHandler;
import helios.engine.mechanics.match.MatchStateTransitionListener;

import helios.engine.mechanics.match.components;

import helios.engine.mechanics.match.commands;
import helios.engine.mechanics.match.types;

import helios.engine.ecs.GameObject;

import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.Session;

import helios.engine.runtime.world.Session;

import helios.core.types;
import helios.util.Guid;

export namespace helios::engine::mechanics::match {

    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::match::commands;

    /**
     * @brief Manages match state transitions using a rule-based state machine.
     *
     * The MatchStateManager processes MatchStateCommands and applies transition rules
     * to move between match states (e.g., Warmup -> PlayerSpawn -> Playing). It supports
     * guard callbacks for conditional transitions and notifies registered listeners
     * on state changes.
     *
     * @see MatchStateTransitionRule
     * @see MatchStateTransitionListener
     */
    class MatchStateManager : public helios::engine::runtime::world::Manager,
                             public MatchStateCommandHandler {



        std::vector<MatchStateCommand> pending_;

        std::vector<std::unique_ptr<MatchStateTransitionListener>> listeners_;


        /**
         * @brief Notifies all registered listeners of a state transition.
         *
         * @param from The state being exited.
         * @param to The state being entered.
         * @param transitionId The identifier of the transition.
         * @param updateContext The current update context.
         *
         * @todo Add support for vetoing transitions.
         */
        void executeListener(
            const MatchState from,
            const MatchState to,
            const MatchStateTransitionId transitionId,
            helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onMatchStateExit(updateContext, from);
                listener->onMatchStateTransition(
                    updateContext,
                    MatchStateTransitionContext{from, to, transitionId}
                );
                listener->onMatchStateEnter(updateContext, to);

            }
        }

        static constexpr MatchStateTransitionRule rules_[] = {
            MatchStateTransitionRule(MatchState::Undefined, MatchStateTransitionId::WarmupRequested, MatchState::Warmup, MatchStateTransitionType::Standard),
            MatchStateTransitionRule(MatchState::Warmup, MatchStateTransitionId::IntroRequested, MatchState::Intro, MatchStateTransitionType::Standard),
            MatchStateTransitionRule(MatchState::Intro, MatchStateTransitionId::CountdownRequested, MatchState::Countdown, MatchStateTransitionType::Standard),
            MatchStateTransitionRule(MatchState::Countdown, MatchStateTransitionId::PlayerSpawnRequested, MatchState::PlayerSpawn, MatchStateTransitionType::Standard),
            MatchStateTransitionRule(MatchState::Warmup, MatchStateTransitionId::PlayerSpawnRequested, MatchState::PlayerSpawn, MatchStateTransitionType::Standard),
            MatchStateTransitionRule(MatchState::PlayerSpawn, MatchStateTransitionId::StartRequested, MatchState::Playing, MatchStateTransitionType::Standard),

        };


    public:

        /**
         * @brief Registers a listener to be notified of state transitions.
         *
         * @param listener The listener to add. Ownership is transferred.
         * @return Reference to this manager for method chaining.
         */
        MatchStateManager& addMatchStateListener(std::unique_ptr<MatchStateTransitionListener> listener) noexcept {
            listeners_.push_back(std::move(listener));
            return *this;
        }

        /**
         * @brief Processes pending state commands and applies valid transitions.
         *
         * Evaluates the last pending command against the configured rules.
         * If a matching rule is found and its guard (if any) passes, the transition
         * is applied and listeners are notified.
         *
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void flush(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext
        ) noexcept override {

            if (pending_.empty()) {
                return;
            }

            auto command = pending_.back();

            auto transitionRequest = command.transitionRequest();

            auto& session = gameWorld.session();
            auto currentFrom = session.matchState();
            auto from = transitionRequest.from();
            auto transitionId = transitionRequest.transitionId();


            if (currentFrom != from) {
                pending_.clear();
                return;
            }


            for (auto& rule : rules_) {
                if (rule.from() == from && rule.transitionId() == transitionId) {

                    if (rule.guard()) {
                        if (!rule.guard()(updateContext, transitionRequest)) {
                            break;
                        }
                    }

                    session.setMatchState(rule.to());
                    executeListener(from, rule.to(), transitionId, updateContext);
                }
            }

            pending_.clear();
        }

        /**
         * @brief Submits a match state command for processing.
         *
         * @param matchStateCommand The command to submit.
         * @return True if the command was accepted.
         */
        bool submit(
            const MatchStateCommand& matchStateCommand
        ) noexcept override {
            pending_.push_back(matchStateCommand);
            return true;
        };


        /**
         * @brief Initializes the manager and registers it with the game world.
         *
         * @param gameWorld The game world to register with.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) override {
            gameWorld.registerMatchStateCommandHandler(*this);
        }

        /**
         * @brief Clears all pending commands.
         */
        void reset() override {
            pending_.clear();
        }

    };

}
