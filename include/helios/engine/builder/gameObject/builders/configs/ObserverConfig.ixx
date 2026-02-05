/**
 * @file ObserverConfig.ixx
 * @brief Configuration for observer components.
 */
module;

#include <cassert>

export module helios.engine.builder.gameObject.builders.configs.ObserverConfig;

import helios.engine.ecs.GameObject;

import helios.engine.core.data;

import helios.engine.mechanics.scoring.components.ScoreObserverComponent;


export namespace helios::engine::builder::gameObject::builders::configs {


    /**
     * @brief Fluent configuration for observer component setup.
     *
     * Provides methods for adding observer components that bind to
     * data sources like score pools.
     */
    class ObserverConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs an ObserverConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit ObserverConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {

        }

        /**
         * @brief Adds a ScoreObserverComponent bound to the specified pool.
         *
         * @param scorePoolId The ID of the score pool to observe.
         *
         * @return Reference to this config for chaining.
         */
        ObserverConfig& scorePool(const helios::engine::core::data::ScorePoolId scorePoolId) {

            auto* soc = gameObject_->get<helios::engine::mechanics::scoring::components::ScoreObserverComponent>();

            assert(!soc && "ScoreObserverComponent already available.");

            gameObject_->add<helios::engine::mechanics::scoring::components::ScoreObserverComponent>()
                       .setScorePoolId(scorePoolId);
            return *this;
        }

    };

}
