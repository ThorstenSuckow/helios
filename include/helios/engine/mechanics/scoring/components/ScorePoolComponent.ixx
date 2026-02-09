/**
 * @file ScorePoolComponent.ixx
 * @brief Component that associates an entity with a score pool.
 */
module;


export module helios.engine.mechanics.scoring.components.ScorePoolComponent;



import helios.engine.core.data;
import helios.core.types;

export namespace helios::engine::mechanics::scoring::components {


    /**
     * @brief Component that associates an entity with a score pool.
     *
     * Attached to entities (e.g., player) to specify which score pool
     * receives points when that entity performs scoring actions.
     */
    class ScorePoolComponent  {

    private:

        /**
         * @brief ID of the associated score pool.
         */
        helios::engine::core::data::ScorePoolId scorePoolId_{helios::core::types::no_init};

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;


    public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        ScorePoolComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        ScorePoolComponent(const ScorePoolComponent& other) : scorePoolId_(other.scorePoolId_)  {}

        ScorePoolComponent& operator=(const ScorePoolComponent&) = default;
        ScorePoolComponent(ScorePoolComponent&&) noexcept = default;
        ScorePoolComponent& operator=(ScorePoolComponent&&) noexcept = default;

        /**
         * @brief Sets the associated score pool.
         *
         * @param scorePoolId The ID of the score pool.
         */
        void setScorePoolId(const helios::engine::core::data::ScorePoolId scorePoolId) noexcept {
            scorePoolId_ = scorePoolId;
        }

        /**
         * @brief Returns the associated score pool ID.
         *
         * @return The ScorePoolId.
         */
        [[nodiscard]] helios::engine::core::data::ScorePoolId scorePoolId() const noexcept {
            return scorePoolId_;
        }

    };


}