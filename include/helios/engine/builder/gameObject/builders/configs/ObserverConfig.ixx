/**
 * @file ObserverConfig.ixx
 * @brief Configuration for observer components.
 */
module;

#include <cassert>

export module helios.engine.builder.gameObject.builders.configs.ObserverConfig;

import helios.engine.ecs.GameObject;

import helios.engine.core.data;

import helios.engine.mechanics.match.components;

import helios.engine.mechanics.scoring.components;
import helios.engine.mechanics.timing;


export namespace helios::engine::builder::gameObject::builders::configs {


    /**
     * @brief Fluent configuration for observer component setup.
     *
     * Provides methods for adding observer components that bind to
     * data sources like score pools and game timers.
     *
     * @see ScoreObserverComponent
     * @see MaxScoreObserverComponent
     * @see GameTimerBindingComponent
     */
    class ObserverConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs an ObserverConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit ObserverConfig(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {

        }

        /**
         * @brief Adds a ScoreObserverComponent bound to the specified pool.
         *
         * @param scorePoolId The ID of the score pool to observe.
         *
         * @return Reference to this config for chaining.
         *
         * @deprecated use runningScore()
         */
        ObserverConfig& scorePool(const helios::engine::core::data::ScorePoolId scorePoolId) {
            return runningScore(scorePoolId);
        }

        /**
         * @brief Adds a ScoreObserverComponent bound to the specified pool.
         *
         * @param scorePoolId The ID of the score pool to observe.
         *
         * @return Reference to this config for chaining.
         */
        ObserverConfig& runningScore(const helios::engine::core::data::ScorePoolId scorePoolId) {

            auto* soc = gameObject_.get<helios::engine::mechanics::scoring::components::ScoreObserverComponent>();

            assert(!soc && "ScoreObserverComponent already available.");

            gameObject_.add<helios::engine::mechanics::scoring::components::ScoreObserverComponent>()
                       .setScorePoolId(scorePoolId);
            return *this;
        }

        /**
         * @brief Adds a LivesBindingComponent observing the given entity's lives.
         *
         * @param gameObject The entity whose LivesComponent is observed.
         *
         * @return Reference to this config for chaining.
         */
        ObserverConfig& lives(const helios::engine::ecs::GameObject gameObject) {

            auto* loc = gameObject_.get<
                helios::engine::mechanics::match::components::LivesBindingComponent
            >();

            assert(!loc && "LivesObserverComponent already available.");

            gameObject_.add<helios::engine::mechanics::match::components::LivesBindingComponent>(
                gameObject.entityHandle()
            );
            return *this;
        }

        /**
         * @brief Adds a MaxScoreObserverComponent bound to the specified pool.
         *
         * @param scorePoolId The ID of the score pool to observe.
         *
         * @return Reference to this config for chaining.
         */
        ObserverConfig& maxScore(const helios::engine::core::data::ScorePoolId scorePoolId) {

            auto* soc = gameObject_.get<helios::engine::mechanics::scoring::components::MaxScoreObserverComponent>();

            assert(!soc && "MaxScoreObserverComponent already available.");

            gameObject_.add<helios::engine::mechanics::scoring::components::MaxScoreObserverComponent>()
                       .setScorePoolId(scorePoolId);
            return *this;
        }

        /**
         * @brief Adds a GameTimerBindingComponent bound to the specified timer.
         *
         * @param gameTimerId The ID of the game timer to observe.
         *
         * @return Reference to this config for chaining.
         */
        ObserverConfig& time(const helios::engine::core::data::GameTimerId gameTimerId) {

            auto* toc = gameObject_.get<helios::engine::mechanics::timing::components::GameTimerBindingComponent>();

            assert(!toc && "GameTimerBindingComponent already available.");

            gameObject_.add<helios::engine::mechanics::timing::components::GameTimerBindingComponent>()
                       .setGameTimerId(gameTimerId);
            return *this;
        }
    };

}
