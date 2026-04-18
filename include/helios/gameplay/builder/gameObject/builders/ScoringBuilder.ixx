/**
 * @file ScoringBuilder.ixx
 * @brief Builder for scoring-related component configuration.
 */
module;

export module helios.gameplay.builder.gameObject.builders.ScoringBuilder;

import helios.gameplay.builder.gameObject.builders.configs.ScorePoolConfig;
import helios.gameplay.builder.gameObject.builders.configs.ScoreValueConfig;

export namespace helios::gameplay::builder::gameObject::builders {

    /**
     * @brief Builder for configuring scoring-related components.
     *
     * Provides methods to create config objects for score pool association
     * and score value components.
     */
    template<typename Entity>
    class ScoringBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a ScoringBuilder for the given GameObject.
         *
         * @param gameObject The target GameObject to configure.
         */
        explicit ScoringBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config object for ScoreValueComponent.
         *
         * @return ScoreValueConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::ScoreValueConfig<Entity> scoreValue() const {
            return helios::gameplay::builder::gameObject::builders::configs::ScoreValueConfig<Entity>{gameObject_};
        }

        /**
         * @brief Returns a config object for ScorePoolComponent.
         *
         * @return ScorePoolConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::ScorePoolConfig<Entity> scorePool() const {
            return helios::gameplay::builder::gameObject::builders::configs::ScorePoolConfig<Entity>{gameObject_};
        }

    };

}
