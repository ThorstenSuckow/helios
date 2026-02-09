/**
 * @file ScoringBuilder.ixx
 * @brief Builder for scoring-related component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.ScoringBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.ScorePoolConfig;
import helios.engine.builder.gameObject.builders.configs.ScoreValueConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring scoring-related components.
     *
     * Provides methods to create config objects for score pool association
     * and score value components.
     */
    class ScoringBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a ScoringBuilder for the given GameObject.
         *
         * @param gameObject The target GameObject to configure.
         */
        explicit ScoringBuilder(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config object for ScoreValueComponent.
         *
         * @return ScoreValueConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::ScoreValueConfig scoreValue() const {
            return helios::engine::builder::gameObject::builders::configs::ScoreValueConfig{gameObject_};
        }

        /**
         * @brief Returns a config object for ScorePoolComponent.
         *
         * @return ScorePoolConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::ScorePoolConfig scorePool() const {
            return helios::engine::builder::gameObject::builders::configs::ScorePoolConfig{gameObject_};
        }

    };

}
