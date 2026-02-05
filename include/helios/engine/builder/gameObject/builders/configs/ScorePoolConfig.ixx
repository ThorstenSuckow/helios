/**
 * @file ScorePoolConfig.ixx
 * @brief Configuration for score pool association components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.ScorePoolConfig;

import helios.engine.ecs.GameObject;

import helios.engine.core.data;

import helios.engine.mechanics.scoring.components.ScorePoolComponent;

export namespace helios::engine::builder::gameObject::builders::configs {


    /**
     * @brief Fluent configuration for ScorePoolComponent setup.
     *
     * Automatically adds ScorePoolComponent and provides methods for
     * setting the associated score pool ID.
     */
    class ScorePoolConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a ScorePoolConfig and adds ScorePoolComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit ScorePoolConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {
            gameObject_->getOrAdd<helios::engine::mechanics::scoring::components::ScorePoolComponent>();
        }

        /**
         * @brief Sets the score pool ID for this entity.
         *
         * @param scorePoolId The ID of the score pool to associate with.
         *
         * @return Reference to this config for chaining.
         */
        ScorePoolConfig& poolId(const helios::engine::core::data::ScorePoolId scorePoolId) {
            gameObject_->get<helios::engine::mechanics::scoring::components::ScorePoolComponent>()
                       ->setScorePoolId(scorePoolId);
            return *this;
        }

    };

}
