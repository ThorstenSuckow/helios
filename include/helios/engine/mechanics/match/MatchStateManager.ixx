/**
 * @file MatchStateManager.ixx
 * @brief Match state manager and transition orchestrator.
 */
module;

#include <span>
#include <memory>
#include <unordered_map>

export module helios.engine.mechanics.match.MatchStateManager;


import helios.engine.mechanics.match.MatchStateCommandHandler;
import helios.engine.mechanics.match.MatchStateTransitionListener;

import helios.engine.mechanics.match.components;

import helios.engine.mechanics.match.commands;
import helios.engine.mechanics.match.types;
import helios.engine.mechanics.match.rules;

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
    using namespace helios::engine::mechanics::match::rules;

    /**
     * @brief Manages match state transitions using a rule-based state machine.
     *
     * @details The MatchStateManager processes MatchStateCommands and applies transition rules
     * to move between match states (e.g., Warmup -> PlayerSpawn -> Playing). It supports
     * guard callbacks for conditional transitions and notifies registered listeners
     * on state changes.
     *
     * ## Transition Flow
     *
     * 1. Commands are submitted via `submit()` and queued
     * 2. On `flush()`, the last pending command is evaluated against rules
     * 3. If a rule matches and its guard passes, the transition executes:
     *    - `onMatchStateExit()` is called on listeners
     *    - `onMatchStateTransition()` is called on listeners
     *    - Session state is updated
     *    - `onMatchStateEnter()` is called on listeners
     *
     * @see MatchStateTransitionRule
     * @see MatchStateTransitionListener
     * @see MatchStateCommand
     * @see LambdaMatchStateListener
     */
    class MatchStateManager : public helios::engine::runtime::world::Manager,
                             public MatchStateCommandHandler {

        /**
         * @brief Queue of pending state commands to process on flush.
         */
        std::vector<MatchStateCommand> pending_;

        /**
         * @brief Registered transition listeners.
         */
        std::vector<std::unique_ptr<MatchStateTransitionListener>> listeners_;

        /**
         * @brief Notifies listeners of state exit.
         *
         * @param from The state being exited.
         * @param to The target state.
         * @param transitionId The transition identifier.
         * @param updateContext The current update context.
         */
        void signalExit(
            const MatchState from,
            const MatchState to,
            const MatchStateTransitionId transitionId,
            helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onMatchStateExit(updateContext, from);
            }
        }

        /**
         * @brief Notifies listeners of state entry.
         *
         * @param from The source state.
         * @param to The state being entered.
         * @param transitionId The transition identifier.
         * @param updateContext The current update context.
         */
        void signalEnter(
            const MatchState from,
            const MatchState to,
            const MatchStateTransitionId transitionId,
            helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onMatchStateEnter(updateContext, to);
            }
        }

        /**
         * @brief Notifies listeners of state transition.
         *
         * @param from The source state.
         * @param to The target state.
         * @param transitionId The transition identifier.
         * @param updateContext The current update context.
         */
        void signalTransition(
            const MatchState from,
            const MatchState to,
            const MatchStateTransitionId transitionId,
            helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onMatchStateTransition(
                    updateContext,
                    MatchStateTransitionContext{from, to, transitionId}
                );
            }
        }

        /**
         * @brief Configured transition rules for the state machine.
         */
        std::vector<MatchStateTransitionRule> rules_;


    public:

        /**
         * @brief Constructs a MatchStateManager with custom transition rules.
         *
         * @param rules A span of transition rules defining valid state transitions.
         */
        explicit MatchStateManager(std::span<const MatchStateTransitionRule> rules)
        : rules_(rules.begin(), rules.end()) {}

        /**
         * @brief Constructs a MatchStateManager with default transition rules.
         */
        MatchStateManager() {
            rules_.assign(DefaultMatchStateTransitionRules::rules().begin(),
                DefaultMatchStateTransitionRules::rules().end()
            );
        }

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

                    // invoke the transitions that can satisfy the guard
                    if (rule.guard()) {
                        // if the guard does not evaluate to true, do not change the state
                        if (!rule.guard()(updateContext, transitionRequest)) {
                            break;
                        }
                    }

                    // clean up
                    signalExit(from, rule.to(), transitionId, updateContext);
                    signalTransition(from, rule.to(), transitionId, updateContext);
                    session.setMatchStateFrom(MatchStateTransitionContext(currentFrom, rule.to(), transitionId));
                    signalEnter(from, rule.to(), transitionId, updateContext);
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
