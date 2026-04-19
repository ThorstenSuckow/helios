/**
 * @file ScorePoolComponent.ixx
 * @brief Component that associates an entity with a score pool.
 */
module;


export module helios.gameplay.scoring.components.ScorePoolComponent;


import helios.gameplay.scoring.types.ScorePoolId;

import helios.core.types.TypeDefs;

export namespace helios::gameplay::scoring::components {


    /**
     * @brief Component that associates an entity with a score pool.
     *
     * Attached to entities (e.g., player) to specify which score pool
     * receives points when that entity performs scoring actions.
     */
    template<typename THandle>
    class ScorePoolComponent  {

    private:

        /**
         * @brief ID of the associated score pool.
         */
        helios::gameplay::scoring::types::ScorePoolId scorePoolId_{helios::core::types::no_init};

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
        void setScorePoolId(const helios::gameplay::scoring::types::ScorePoolId scorePoolId) noexcept {
            scorePoolId_ = scorePoolId;
        }

        /**
         * @brief Returns the associated score pool ID.
         *
         * @return The ScorePoolId.
         */
        [[nodiscard]] helios::gameplay::scoring::types::ScorePoolId scorePoolId() const noexcept {
            return scorePoolId_;
        }

    };


}