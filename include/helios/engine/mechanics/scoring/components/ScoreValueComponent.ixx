/**
 * @file ScoreValueComponent.ixx
 * @brief Template component for storing score values on entities.
 */
module;

#include <type_traits>

export module helios.engine.mechanics.scoring.components.ScoreValueComponent;



import helios.engine.mechanics.scoring.types.Score;

export namespace helios::engine::mechanics::scoring::components {

    /**
     * @brief Template component that stores a score value of a specific type.
     *
     * Attached to entities to define the score value awarded when that entity
     * is involved in a scoring event (e.g., enemy killed). The template parameter
     * must derive from Score.
     *
     * @tparam T The score type (must derive from Score).
     */
    template<typename T>
    requires std::is_base_of_v<helios::engine::mechanics::scoring::types::Score, T>
    class ScoreValueComponent  {

    private:

        /**
         * @brief The score value instance.
         */
        T score_{};

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

        /**
         * @brief Constructs a ScoreValueComponent with forwarded arguments.
         *
         * @tparam Args Argument types for Score construction.
         * @param args Arguments forwarded to the Score constructor.
         */
        template<typename... Args>
        explicit ScoreValueComponent(Args&&... args) : score_(std::forward<Args>(args)...) {}

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        ScoreValueComponent(const ScoreValueComponent& other) : score_(other.score_) {}

        ScoreValueComponent& operator=(const ScoreValueComponent& other) = default;

        ScoreValueComponent(ScoreValueComponent&& other) noexcept = default;
        ScoreValueComponent& operator=(ScoreValueComponent&& other) noexcept = default;



        /**
         * @brief Returns the stored score value.
         *
         * @return The score instance.
         */
        [[nodiscard]] T score() const noexcept {
            return score_;
        }

    };


}