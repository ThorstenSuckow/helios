/**
 * @file KillReward.ixx
 * @brief Score type awarded for killing an entity.
 */
module;


export module helios.gameplay.scoring.types.KillReward;

import helios.gameplay.scoring.types.Score;
import helios.gameplay.scoring.types.ScoreTypeId;


export namespace helios::gameplay::scoring::types {


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
        [[nodiscard]] ScoreTypeId typeId() const noexcept override {
            return ScoreTypeId::id<KillReward>();
        };

    };


}