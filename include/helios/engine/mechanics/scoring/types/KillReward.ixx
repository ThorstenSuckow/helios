/**
 * @file KillReward.ixx
 * @brief Score type awarded for killing an entity.
 */
module;


export module helios.engine.mechanics.scoring.types.KillReward;

import helios.engine.mechanics.scoring.types.Score;

import helios.engine.core.data;

export namespace helios::engine::mechanics::scoring::types {


    /**
     * @brief Score type awarded when an entity is killed.
     *
     * Used by the combat scoring system to award points when
     * an entity with a KillReward ScoreValueComponent is destroyed.
     */
    class KillReward: public Score {


    public:

        /**
         * @brief Constructs a KillReward with the given score value.
         *
         * @param score The points awarded for the kill.
         */
        explicit KillReward(const double score) : Score(score){}

        /**
         * @copydoc Score::typeId()
         */
        [[nodiscard]] helios::engine::core::data::ScoreTypeId typeId() const noexcept override {
            return helios::engine::core::data::ScoreTypeId::id<KillReward>();
        };

    };


}