/**
 * @file StateManager.ixx
 * @brief Generic state manager and transition orchestrator.
 */
module;

#include <memory>
#include <span>
#include <vector>


export module helios.engine.state.StateManager;


import helios.engine.state.TypedStateCommandHandler;
import helios.engine.state.StateTransitionListener;
import helios.engine.state.types.StateTransitionRule;

import helios.engine.state.components;
import helios.engine.state.commands;
import helios.engine.state.types;

import helios.engine.ecs.GameObject;

import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.Session;

import helios.engine.runtime.world.Session;

import helios.core.types;
import helios.util.Guid;

export namespace helios::engine::state {

    using namespace helios::engine::state::types;
    using namespace helios::engine::state::commands;

    /**
     * @brief Manages state transitions using a rule-based system.
     *
     * @details Implements both Manager and TypedStateCommandHandler interfaces.
     * Processes state transition commands by matching against registered rules,
     * executing guards, and notifying listeners.
     *
     * ## Transition Flow
     *
     * 1. Commands are submitted via submit() and queued
     * 2. During flush(), the last pending command is processed
     * 3. Rules are checked to find a matching transition
     * 4. If a guard is present, it must return true
     * 5. Listeners are notified: onStateExit -> onStateTransition -> onStateEnter
     * 6. Session state is updated
     *
     * @tparam StateType The state enum type.
     *
     * @see StateTransitionRule
     * @see StateTransitionListener
     * @see StateCommand
     */
    template <typename StateType>
    class StateManager : public helios::engine::runtime::world::Manager,
                             public TypedStateCommandHandler<StateType> {

        /**
         * @brief Queue of pending state commands.
         */
        std::vector<StateCommand<StateType>> pending_;

        /**
         * @brief Registered transition listeners.
         */
        std::vector<std::unique_ptr<StateTransitionListener<StateType>>> listeners_;

        /**
         * @brief Transition rules defining valid state changes.
         */
        std::vector<StateTransitionRule<StateType>> rules_;

        /**
         * @brief Notifies listeners of state exit.
         *
         * @param from The state being exited.
         * @param to The target state.
         * @param transitionId The transition identifier.
         * @param updateContext The current frame's update context.
         */
        void signalExit(
            const StateType from,
            const StateType to,
            const StateTransitionIdType<StateType> transitionId,
            helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onStateExit(updateContext, from);
            }
        }

        /**
         * @brief Notifies listeners of the transition.
         *
         * @param from The source state.
         * @param to The target state.
         * @param transitionId The transition identifier.
         * @param updateContext The current frame's update context.
         */
        void signalTransition(
           const StateType from,
           const StateType to,
           const StateTransitionIdType<StateType> transitionId,
           helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onStateTransition(
                    updateContext,
                    StateTransitionContext<StateType>{from, to, transitionId}
                );
            }
        }

        /**
         * @brief Notifies listeners of state entry.
         *
         * @param from The source state.
         * @param to The state being entered.
         * @param transitionId The transition identifier.
         * @param updateContext The current frame's update context.
         */
        void signalEnter(
           const StateType from,
           const StateType to,
           const StateTransitionIdType<StateType> transitionId,
           helios::engine::runtime::world::UpdateContext& updateContext)  {

            for (auto& listener : listeners_) {
                listener->onStateEnter(updateContext, to);

            }
        }


    public:

        /**
         * @brief Constructs a state manager with transition rules.
         *
         * @param rules Span of valid transition rules.
         */
        explicit StateManager(std::span<const StateTransitionRule<StateType>> rules)
        : rules_(rules.begin(), rules.end()) {}

        /**
         * @brief Registers a state transition listener.
         *
         * @param listener The listener to add.
         *
         * @return Reference to this manager for chaining.
         */
        StateManager& addStateListener(std::unique_ptr<StateTransitionListener<StateType>> listener) noexcept {
            listeners_.push_back(std::move(listener));
            return *this;
        }

        /**
         * @brief Processes pending state commands.
         *
         * @details Processes the last pending command, finds matching rules,
         * executes guards, and triggers the transition if valid.
         *
         * @param gameWorld The game world.
         * @param updateContext The current frame's update context.
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
            auto currentFrom = session.state<StateType>();
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
                    session.setStateFrom<StateType>(StateTransitionContext<StateType>{rule.from(), rule.to(), transitionId});
                    signalEnter(from, rule.to(), transitionId, updateContext);
                }
            }

            pending_.clear();
        }

        /**
         * @brief Submits a state command for processing.
         *
         * @param stateCommand The command to queue.
         *
         * @return True (always accepts commands).
         */
        bool submit(
            const StateCommand<StateType>& stateCommand
        ) noexcept override {
            pending_.push_back(stateCommand);
            return true;
        };

        /**
         * @brief Initializes the manager and registers with GameWorld.
         *
         * @param gameWorld The game world to register with.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) override {
            gameWorld.registerStateCommandHandler<StateType>(*this);
        }

        /**
         * @brief Clears all pending commands.
         */
        void reset() override {
            pending_.clear();
        }

    };

}
