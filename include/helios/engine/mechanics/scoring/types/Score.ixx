/**
 * @file Score.ixx
 * @brief Abstract base class for score types.
 */
module;


export module helios.engine.mechanics.scoring.types.Score;

import helios.engine.core.data;

export namespace helios::engine::mechanics::scoring::types {


    /**
     * @brief Abstract base class for score value types.
     *
     * Provides a common interface for different score types (e.g., KillReward).
     * Each concrete type must provide a unique ScoreTypeId.
     */
    class Score {

    protected:

        /**
         * @brief The numeric score value.
         */
        double value_;

    public:

        virtual ~Score() = default;

        /**
         * @brief Constructs a Score with the given value.
         *
         * @param value The numeric score value.
         */
        explicit Score(const double value) : value_(value){}

        /**
         * @brief Returns the score value.
         *
         * @return The numeric score value.
         */
        [[nodiscard]] double value() const noexcept {
            return value_;
        }

        /**
         * @brief Returns the unique type identifier for this score type.
         *
         * @return The ScoreTypeId for this concrete score type.
         */
        [[nodiscard]] virtual helios::engine::core::data::ScoreTypeId typeId() const noexcept = 0;

    };


}