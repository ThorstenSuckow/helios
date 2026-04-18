/**
 * @file ScorePoolConfig.ixx
 * @brief Configuration for score pool association components.
 */
module;

export module helios.gameplay.builder.gameObject.builders.configs.ScorePoolConfig;


import helios.gameplay.scoring.types;
import helios.gameplay.scoring.components.ScorePoolComponent;

export namespace helios::gameplay::builder::gameObject::builders::configs {


    /**
     * @brief Fluent configuration for ScorePoolComponent setup.
     *
     * Automatically adds ScorePoolComponent and provides methods for
     * setting the associated score pool ID.
     *
     * @tparam Entity The entity type (e.g., GameObject).
     */
    template<typename Entity>
    class ScorePoolConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target entity.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a ScorePoolConfig and adds ScorePoolComponent.
         *
         * @param gameObject Target entity to configure.
         */
        explicit ScorePoolConfig(Entity gameObject) : gameObject_(gameObject) {
            gameObject_.template getOrAdd<helios::gameplay::scoring::components::ScorePoolComponent<Handle_type>>();
        }

        /**
         * @brief Sets the score pool ID for this entity.
         *
         * @param scorePoolId The ID of the score pool to associate with.
         *
         * @return Reference to this config for chaining.
         */
        ScorePoolConfig& poolId(const helios::gameplay::scoring::types::ScorePoolId scorePoolId) {
            gameObject_.template get<helios::gameplay::scoring::components::ScorePoolComponent<Handle_type>>()
                       ->setScorePoolId(scorePoolId);
            return *this;
        }

    };

}
