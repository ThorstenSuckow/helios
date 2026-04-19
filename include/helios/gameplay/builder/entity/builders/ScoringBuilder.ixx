/**
 * @file ScoringBuilder.ixx
 * @brief Builder for scoring-related component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.ScoringBuilder;

import helios.gameplay.builder.entity.builders.configs.ScorePoolConfig;
import helios.gameplay.builder.entity.builders.configs.ScoreValueConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring scoring-related components.
     *
     * Provides methods to create config objects for score pool association
     * and score value components.
     */
    template<typename Entity>
    class ScoringBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a ScoringBuilder for the given Entity.
         *
         * @param entity The target Entity to configure.
         */
        explicit ScoringBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config object for ScoreValueComponent.
         *
         * @return ScoreValueConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::ScoreValueConfig<Entity> scoreValue() const {
            return helios::gameplay::builder::entity::builders::configs::ScoreValueConfig<Entity>{entity_};
        }

        /**
         * @brief Returns a config object for ScorePoolComponent.
         *
         * @return ScorePoolConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::ScorePoolConfig<Entity> scorePool() const {
            return helios::gameplay::builder::entity::builders::configs::ScorePoolConfig<Entity>{entity_};
        }

    };

}
