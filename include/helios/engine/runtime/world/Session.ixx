/**
 * @file Session.ixx
 * @brief Session state container for game and match state.
 */
module;

#include <optional>
#include <vector>
#include <span>

export module helios.engine.runtime.world.Session;

import helios.engine.mechanics.gamestate.types;

import helios.engine.mechanics.match.types;
import helios.engine.mechanics.match.components;

import helios.engine.core.data;

import helios.engine.mechanics.gamestate.components;
import helios.engine.ecs.GameObject;

import helios.engine.modules.ui.widgets.components.UiFocusComponent;

import helios.engine.core.data.ViewportId;

import helios.engine.modules.rendering.viewport.components.ActiveViewportIdsStateComponent;

import helios.engine.mechanics.match.components;

export namespace helios::engine::runtime::world {

    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::match::components;

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::gamestate::components;

    using namespace helios::engine::modules::rendering::viewport::components;

    /**
     * @brief Holds session-level state for the current game instance.
     *
     * The Session wraps a GameObject that stores session-related components
     * such as GameStateComponent, MatchStateComponent, and UiFocusComponent.
     * It provides convenient accessors for game state, match state, and UI focus.
     *
     * @see GameStateComponent
     * @see MatchStateComponent
     */
    class Session {

        ecs::GameObject gameObject_;

        std::optional<ecs::GameObject> playerEntity_;

    public:

        /**
         * @brief Constructs a session with the given GameObject.
         *
         * Automatically adds GameStateComponent, UiFocusComponent, and
         * MatchStateComponent to the GameObject.
         *
         * @param go The GameObject to use as the session entity.
         */
        explicit Session(const ecs::GameObject go) : gameObject_(go) {

            gameObject_.add<GameStateComponent>();
            gameObject_.add<modules::ui::widgets::components::UiFocusComponent>();
            gameObject_.add<ActiveViewportIdsStateComponent>();
            gameObject_.add<MatchStateComponent>();

        }

        /**
         * @brief Sets the player entity for this session.
         *
         * @param go The GameObject representing the player.
         */
        void setPlayerEntity(const ecs::GameObject go) noexcept {
            playerEntity_ = go;
        }

        /**
         * @brief Returns the player entity if set.
         *
         * @return Optional containing the player GameObject, or nullopt.
         */
        [[nodiscard]] std::optional<ecs::GameObject> playerEntity() const noexcept {
            return playerEntity_;
        }


        /**
         * @brief Resets the session state.
         */
        void reset() {


        }

        /**
         * @brief Checks if the game is currently paused.
         *
         * @return True if the game state is Paused.
         */
        [[nodiscard]] bool isPaused() const noexcept {
            auto* gpc = gameObject_.get<GameStateComponent>();

            return gpc && gpc->gameState == GameState::Paused;
        }

        /**
         * @brief Sets the current game state.
         *
         * @param gameState The new game state.
         */
        void setGameState(const GameState gameState) noexcept {
            auto* gpc = gameObject_.get<GameStateComponent>();

            if (gpc) {
                gpc->gameState = gameState;
            }
        }

        /**
         * @brief Sets the current match state.
         *
         * @param matchStateTransitionContext
         */
        void setMatchStateFrom(const MatchStateTransitionContext matchStateTransitionContext) noexcept {

            if (auto* msc = gameObject_.get<MatchStateComponent>()) {
                msc->setMatchState(matchStateTransitionContext);
            }
        }

        /**
         * @brief Returns the current game state.
         *
         * @return The current game state, or Undefined if not available.
         */
        [[nodiscard]] GameState gameState() const noexcept {
            auto* gpc = gameObject_.get<GameStateComponent>();

            return gpc ? gpc->gameState : GameState::Undefined;
        }

        /**
         * @brief Returns the current match state.
         *
         * @return The current match state, or Undefined if not available.
         */
        [[nodiscard]] MatchState matchState() const noexcept {
            auto* ms = gameObject_.get<MatchStateComponent>();

            return ms ? ms->matchState() : MatchState::Undefined;
        }

        /**
         * @brief Returns the current match state transition ID.
         *
         * @return The transition ID, or Undefined if not available.
         */
        [[nodiscard]] MatchStateTransitionId matchStateTransitionId() const noexcept {
            auto* ms = gameObject_.get<MatchStateComponent>();

            return ms ? ms->matchStateTransitionId() : MatchStateTransitionId::Undefined;
        }


        /**
         * @brief Sets the currently focused UI entity.
         *
         * @param gameObject The entity to focus, or nullopt to clear focus.
         */
        void setFocusedEntity(std::optional<helios::engine::ecs::GameObject> gameObject) noexcept {
            gameObject_.get<modules::ui::widgets::components::UiFocusComponent>()->setFocusedEntity(gameObject);
        }

        /**
         * @brief Returns the currently focused UI entity.
         *
         * @return The focused entity, or nullopt if none is focused.
         */
        std::optional<helios::engine::ecs::GameObject> focusedEntity() noexcept {
            return gameObject_.get<modules::ui::widgets::components::UiFocusComponent>()->focusedEntity();
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
        std::span<const helios::engine::core::data::ViewportId> viewportIds() const noexcept {
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