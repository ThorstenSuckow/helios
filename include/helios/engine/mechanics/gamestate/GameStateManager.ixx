/**
 * @file GameStateManager.ixx
 * @brief Game state manager and transition orchestrator.
 */
module;

#include <memory>
#include <span>
#include <vector>


export module helios.engine.mechanics.gamestate.GameStateManager;


import helios.engine.mechanics.gamestate.GameStateCommandHandler;
import helios.engine.mechanics.gamestate.GameStateTransitionListener;

import helios.engine.mechanics.gamestate.components;

import helios.engine.mechanics.gamestate.rules;
import helios.engine.mechanics.gamestate.commands;
import helios.engine.mechanics.gamestate.types;

import helios.engine.ecs.GameObject;

import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.Session;

import helios.engine.runtime.world.Session;

import helios.core.types;
import helios.util.Guid;

export namespace helios::engine::mechanics::gamestate {

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::gamestate::commands;
    using namespace helios::engine::mechanics::gamestate::rules;

    /**
     * @brief Manages game state transitions using a rule-based state machine.
     *
     * @details The GameStateManager processes GameStateCommands and applies transition rules
     * to move between game states (e.g., Title -> Running -> Paused). It supports
     * guard callbacks for conditional transitions and notifies registered listeners
     * on state changes.
     *
     * ## Transition Flow
     *
     * 1. Commands are submitted via `submit()` and queued
     * 2. On `flush()`, the last pending command is evaluated against rules
     * 3. If a rule matches and its guard passes, the transition executes:
     *    - `onGameStateExit()` is called on listeners
     *    - `onGameStateTransition()` is called on listeners
     *    - Session state is updated
     *    - `onGameStateEnter()` is called on listeners
     *
     * @see GameStateTransitionRule
     * @see GameStateTransitionListener
     * @see GameStateCommand
     * @see LambdaGameStateListener
     */
    class GameStateManager : public helios::engine::runtime::world::Manager,
                             public GameStateCommandHandler {

        /**
         * @brief Queue of pending state commands to process on flush.
         */
        std::vector<GameStateCommand> pending_;

        /**
         * @brief Registered transition listeners.
         */
        std::vector<std::unique_ptr<GameStateTransitionListener>> listeners_;

        /**
         * @brief Notifies listeners of state exit.
         *
         * @param from The state being exited.
         * @param to The target state.
         * @param transitionId The transition identifier.
         * @param updateContext The current update context.
         */
        void signalExit(
            const GameState from,
            const GameState to,
            const GameStateTransitionId transitionId,
            helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onGameStateExit(updateContext, from);
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
           const GameState from,
           const GameState to,
           const GameStateTransitionId transitionId,
           helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onGameStateTransition(
                    updateContext,
                    GameStateTransitionContext{from, to, transitionId}
                );
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
           const GameState from,
           const GameState to,
           const GameStateTransitionId transitionId,
           helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onGameStateEnter(updateContext, to);

            }
        }

        /**
         * @brief Configured transition rules for the state machine.
         */
        std::vector<GameStateTransitionRule> rules_;

    public:

        /**
         * @brief Constructs a GameStateManager with custom transition rules.
         *
         * @param rules A span of transition rules defining valid state transitions.
         */
        explicit GameStateManager(std::span<const GameStateTransitionRule> rules)
        : rules_(rules.begin(), rules.end()) {}

        /**
         * @brief Constructs a GameStateManager with default transition rules.
         */
        GameStateManager() {
            rules_.assign(rules
                ::DefaultGameStateTransitionRules::rules().begin(),
                rules::DefaultGameStateTransitionRules::rules().end()
            );
        }

        /**
         * @brief Registers a listener to be notified of state transitions.
         *
         * @param listener The listener to add. Ownership is transferred.
         * @return Reference to this manager for method chaining.
         */
        GameStateManager& addGameStateListener(std::unique_ptr<GameStateTransitionListener> listener) noexcept {
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
            auto currentFrom = session.gameState();
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

                    signalExit(from, rule.to(), transitionId, updateContext);
                    signalTransition(from, rule.to(), transitionId, updateContext);
                    session.setGameState(rule.to());
                    signalEnter(from, rule.to(), transitionId, updateContext);
                }
            }

            pending_.clear();
        }


        /**
         * @brief Submits a state transition command for processing.
         *
         * @param gameStateCommand The command to submit.
         * @return True if the command was accepted.
         */
        bool submit(
            const GameStateCommand& gameStateCommand
        ) noexcept override {
            pending_.push_back(gameStateCommand);
            return true;
        };


        /**
         * @brief Initializes the manager and registers it with the game world.
         *
         * @param gameWorld The game world to register with.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) override {
            gameWorld.registerGameStateCommandHandler(*this);
        }

        /**
         * @brief Clears all pending commands.
         */
        void reset() override {
            pending_.clear();
        }

    };

}
