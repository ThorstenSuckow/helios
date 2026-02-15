/**
 * @file Session.ixx
 * @brief Session state container for game and match state.
 */
module;

#include <optional>

export module helios.engine.runtime.world.Session;

import helios.engine.mechanics.gamestate.types;

import helios.engine.mechanics.match.types;
import helios.engine.mechanics.match.components;

import helios.engine.core.data;

import helios.engine.mechanics.gamestate.components;
import helios.engine.ecs.GameObject;

import helios.engine.modules.ui.widgets.components.UiFocusComponent;

import helios.engine.mechanics.match.components;

export namespace helios::engine::runtime::world {

    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::match::components;

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::gamestate::components;

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
            gameObject_.add<MatchStateComponent>();

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
         * @param matchState The new match state.
         */
        void setMatchState(const MatchState matchState) noexcept {
            auto* msc = gameObject_.get<MatchStateComponent>();

            if (msc) {
                msc->setMatchState(matchState);
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
    ;
        }
    };


}