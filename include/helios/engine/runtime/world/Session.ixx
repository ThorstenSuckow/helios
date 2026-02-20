/**
 * @file Session.ixx
 * @brief Session state container for game-wide state.
 */
module;

#include <optional>
#include <vector>
#include <span>
#include <cassert>

export module helios.engine.runtime.world.Session;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.state.types;
import helios.engine.state.components;
import helios.engine.state.types.StateTransitionId;

import helios.engine.core.data;

import helios.engine.ecs.GameObject;
import helios.engine.ecs.EntityHandle;

import helios.engine.core.data.ViewportId;

import helios.engine.modules.rendering.viewport.components.ActiveViewportIdsStateComponent;

export namespace helios::engine::runtime::world {

    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::gamestate::types;

    using namespace helios::engine::state::types;
    using namespace helios::engine::state::components;

    using namespace helios::engine::modules::rendering::viewport::components;

    /**
     * @brief Holds session-level state for the current game instance.
     *
     * @details The Session wraps a GameObject that stores session-related
     * components using the template-based state system. It provides type-safe
     * accessors for any registered state type (e.g., GameState, MatchState).
     *
     * State types must be registered via trackState<T>() before use:
     *
     * ```cpp
     * session.trackState<GameState>();
     * session.trackState<MatchState>();
     * ```
     *
     * ## Stored Components
     *
     * - `StateComponent<T>` - Registered via trackState<T>()
     * - `ActiveViewportIdsStateComponent` - Active viewport list (auto-added)
     *
     * @see StateComponent
     * @see StateManager
     */
    class Session {

        /**
         * @brief The underlying GameObject storing session components.
         */
        ecs::GameObject gameObject_;

        /**
         * @brief Handle to the player entity.
         */
        ecs::EntityHandle playerEntity_;

    public:

        /**
         * @brief Constructs a session with the given GameObject.
         *
         * @details Automatically adds ActiveViewportIdsStateComponent.
         * State types must be registered separately via trackState<T>().
         *
         * @param go The GameObject to use as the session entity.
         */
        explicit Session(const ecs::GameObject go) : gameObject_(go) {
            gameObject_.add<ActiveViewportIdsStateComponent>();
        }

        /**
         * @brief Sets the player entity handle.
         *
         * @param go The player's entity handle.
         */
        void setPlayerEntityHandle(const ecs::EntityHandle go) noexcept {
            playerEntity_ = go;
        }

        /**
         * @brief Returns the player entity handle.
         *
         * @return The player's entity handle.
         */
        [[nodiscard]] ecs::EntityHandle playerEntityHandle() const noexcept {
            return playerEntity_;
        }

        /**
         * @brief Resets the session state.
         */
        void reset() {

        }

        /**
         * @brief Updates state from a transition context.
         *
         * @details Called by StateManager after a successful transition.
         *
         * @tparam StateType The state enum type.
         *
         * @param stateTransitionContext The completed transition context.
         */
        template<typename StateType>
        void setStateFrom(const StateTransitionContext<StateType> stateTransitionContext) noexcept {

            if (auto* msc = gameObject_.get<StateComponent<StateType>>()) {
                msc->setStateFromTransitionContext(stateTransitionContext);
            }
        }

        /**
         * @brief Returns the current state for a given state type.
         *
         * @tparam StateType The state enum type.
         *
         * @return The current state, or StateType::Undefined if not found.
         */
        template<typename StateType>
        [[nodiscard]] StateType state() const noexcept {
            auto* sc = gameObject_.get<StateComponent<StateType>>();

            return sc ? sc->state() : StateType::Undefined;
        }

        /**
         * @brief Returns the last transition ID for a given state type.
         *
         * @tparam StateType The state enum type.
         *
         * @return The transition ID, or Undefined if not found.
         */
        template<typename StateType>
        [[nodiscard]] StateTransitionIdType<StateType> stateTransitionId() const noexcept {
            auto* ms = gameObject_.get<StateComponent<StateType>>();

            return ms ? ms->transitionId() : StateTransitionIdType<StateType>::Undefined;
        }

        /**
         * @brief Lets this session track the specified StateType.
         *
         * @tparam StateType The state enum type.
         */
        template<typename StateType>
        void trackState() {
            gameObject_.getOrAdd<StateComponent<StateType>>();
        }

        /**
         * @brief Replaces the active viewport IDs with the provided list.
         *
         * @param viewportIds The new list of active viewport IDs.
         */
        void setViewportIds(std::span<const helios::engine::core::data::ViewportId>& viewportIds) noexcept {
            gameObject_.get<ActiveViewportIdsStateComponent>()->setViewportIds(viewportIds);
        }

        /**
         * @brief Returns the currently active viewport IDs.
         *
         * @return Read-only span of viewport identifiers.
         */
        [[nodiscard]] std::span<const helios::engine::core::data::ViewportId> viewportIds() const noexcept {
            return gameObject_.get<ActiveViewportIdsStateComponent>()->viewportIds();
        }

        /**
         * @brief Clears all active viewport IDs.
         */
       void clearViewportIds() noexcept {
            gameObject_.get<ActiveViewportIdsStateComponent>()->clear();
        }
    };


}