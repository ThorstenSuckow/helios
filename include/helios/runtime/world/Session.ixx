/**
 * @file Session.ixx
 * @brief Session state container for game-wide state.
 */
module;

#include <optional>
#include <vector>
#include <span>
#include <cassert>

export module helios.runtime.world.Session;


import helios.gameplay.lifecycle.components;
import helios.gameplay.gamestate.types;
import helios.gameplay.matchstate.types;

import helios.state.types;
import helios.state.components;
import helios.state.types.StateTransitionId;

import helios.runtime.world.GameObject;
import helios.ecs.types.EntityHandle;

import helios.gameplay.lifecycle.components;

import helios.rendering.viewport.types.ViewportHandle;

import helios.rendering.viewport.components.ActiveViewportHandlesStateComponent;

using namespace helios::gameplay::matchstate::types;
using namespace helios::gameplay::gamestate::types;


using namespace helios::state::types;
using namespace helios::state::components;

using namespace helios::rendering::viewport::components;

using namespace helios::rendering::viewport::types;
using namespace helios::gameplay::lifecycle::components;
using namespace helios::runtime::world;
export namespace helios::runtime::world {

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

        using Handle_type = GameObject::Handle_type;

        /**
         * @brief The underlying GameObject storing session components.
         */
        GameObject gameObject_;

        /**
         * @brief Handle to the player entity.
         */
        Handle_type playerEntity_;

    public:

        /**
         * @brief Constructs a session with the given GameObject.
         *
         * @details Automatically adds ActiveViewportHandlesStateComponent.
         * State types must be registered separately via trackState<T>().
         *
         * @param go The GameObject to use as the session entity.
         */
        explicit Session(const GameObject go) : gameObject_(go) {
            gameObject_.add<ActiveViewportHandlesStateComponent<Handle_type>>();
            gameObject_.add<UninitializedComponent<Handle_type>>();
        }

        [[nodiscard]] bool isInitialized() const noexcept {
            return !gameObject_.has<UninitializedComponent<Handle_type>>();
        }

        [[nodiscard]] bool initialize() noexcept {
            return gameObject_.remove<UninitializedComponent<Handle_type>>();
        }

        [[nodiscard]] bool isDestroyed() noexcept {
            return gameObject_.has<DestroyedComponent<Handle_type>>();
        }

        void destroy() noexcept {
            gameObject_.add<DestroyedComponent<Handle_type>>();
        }

        /**
         * @brief Sets the player entity handle.
         *
         * @param go The player's entity handle.
         */
        void setPlayerEntityHandle(const Handle_type go) noexcept {
            playerEntity_ = go;
        }

        /**
         * @brief Returns the player entity handle.
         *
         * @return The player's entity handle.
         */
        [[nodiscard]] Handle_type playerEntityHandle() const noexcept {
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
         * @brief Returns the source state of the last transition.
         *
         * @tparam StateType The state enum type.
         *
         * @return The state that was transitioned from, or StateType::Undefined if not found.
         */
        template<typename StateType>
        [[nodiscard]] StateType stateFrom() const noexcept {
            auto* sc = gameObject_.get<StateComponent<StateType>>();

            return sc ? sc->from() : StateType::Undefined;
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
         * @param viewportHandles The new list of active viewport IDs.
         */
        void setViewportHandles(std::span<const ViewportHandle>& viewportHandles) noexcept {
            gameObject_.get<ActiveViewportHandlesStateComponent<Handle_type>>()->setViewportHandles(viewportHandles);
        }

        /**
         * @brief Returns the currently active viewport handles.
         *
         * @return Read-only span of viewport handles.
         */
        [[nodiscard]] std::span<const ViewportHandle> viewportHandles() const noexcept {
            return gameObject_.get<ActiveViewportHandlesStateComponent<Handle_type>>()->viewportHandles();
        }

        /**
         * @brief Returns true if the specified ViewportHandle is currently active.
         *
         * @param viewportHandle The ViewportHandle to check for activity.
         *
         * @return true if the ViewportHandle is considered active, otherwise false.
         */
        [[nodiscard]] bool isActiveViewport(const ViewportHandle viewportHandle) const noexcept {
            return gameObject_.get<ActiveViewportHandlesStateComponent<Handle_type>>()->has(viewportHandle);
        }

        /**
         * @brief Clears all active viewport handles.
         */
       void clearViewportHandles() noexcept {
            gameObject_.get<ActiveViewportHandlesStateComponent<Handle_type>>()->clear();
        }
    };


}