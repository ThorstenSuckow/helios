/**
 * @file ObserverConfig.ixx
 * @brief Configuration for observer components.
 */
module;

#include <cassert>

export module helios.gameplay.builder.gameObject.builders.configs.ObserverConfig;



import helios.gameplay.matchstate.components;

import helios.gameplay.scoring.components;
import helios.gameplay.scoring.types;
import helios.gameplay.timing;

export namespace helios::gameplay::builder::gameObject::builders::configs {

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
    template<typename Entity>
    class ObserverConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs an ObserverConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit ObserverConfig(Entity gameObject) : gameObject_(gameObject) {

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
        ObserverConfig& scorePool(const helios::gameplay::scoring::types::ScorePoolId scorePoolId) {
            return runningScore(scorePoolId);
        }

        /**
         * @brief Adds a ScoreObserverComponent bound to the specified pool.
         *
         * @param scorePoolId The ID of the score pool to observe.
         *
         * @return Reference to this config for chaining.
         */
        ObserverConfig& runningScore(const helios::gameplay::scoring::types::ScorePoolId scorePoolId) {

            auto* soc = gameObject_.get<helios::gameplay::scoring::components::ScoreObserverComponent<Handle_type>>();

            assert(!soc && "ScoreObserverComponent already available.");

            gameObject_.template add<helios::gameplay::scoring::components::ScoreObserverComponent<Handle_type>>()
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
        ObserverConfig& lives(const Entity gameObject) {

            auto* loc = gameObject_.get<
                helios::gameplay::matchstate::components::LivesBindingComponent<Handle_type>
            >();

            assert(!loc && "LivesBindingComponent already available.");

            gameObject_.template add<helios::gameplay::matchstate::components::LivesBindingComponent<Handle_type>>(
                gameObject.handle()
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
        ObserverConfig& maxScore(const helios::gameplay::scoring::types::ScorePoolId scorePoolId) {

            auto* soc = gameObject_.get<helios::gameplay::scoring::components::MaxScoreObserverComponent<Handle_type>>();

            assert(!soc && "MaxScoreObserverComponent already available.");

            gameObject_.template add<helios::gameplay::scoring::components::MaxScoreObserverComponent<Handle_type>>()
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
        ObserverConfig& time(const helios::gameplay::timing::types::GameTimerId gameTimerId) {

            auto* toc = gameObject_.get<helios::gameplay::timing::components::GameTimerBindingComponent<Handle_type>>();

            assert(!toc && "GameTimerBindingComponent already available.");

            gameObject_.template add<helios::gameplay::timing::components::GameTimerBindingComponent<Handle_type>>()
                       .setGameTimerId(gameTimerId);
            return *this;
        }
    };

}
