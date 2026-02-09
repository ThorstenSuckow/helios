/**
 * @file TranslationStateComponent.ixx
 * @brief Component for storing the current translation (position) state of an entity.
 */
module;

#include <cmath>

export module helios.engine.modules.spatial.transform.components.TranslationStateComponent;



import helios.math;

export namespace helios::engine::modules::spatial::transform::components {

    /**
     * @brief Component that holds the current translation (position) state.
     *
     * @details
     * This component stores the accumulated translation vector. It acts as a data
     * container for the ComposeTransformSystem to build the final translation matrix.
     * It separates the raw position data from the computed ComposeTransformComponent.
     */
    class TranslationStateComponent {

        /**
         * @brief The current translation vector.
         */
        helios::math::vec3f translation_{};

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
         * @brief Default constructor.
         */
        TranslationStateComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        TranslationStateComponent(const TranslationStateComponent& other) :
            translation_(other.translation_){
        }

        /**
         * @brief Copy assignment operator.
         */
        TranslationStateComponent& operator=(const TranslationStateComponent&) = default;

        /**
         * @brief Move constructor.
         */
        TranslationStateComponent(TranslationStateComponent&&) noexcept = default;

        /**
         * @brief Move assignment operator.
         */
        TranslationStateComponent& operator=(TranslationStateComponent&&) noexcept = default;

        /**
         * @brief Creates a new TranslationStateComponent with the specified translation vector.
         *
         * @param translation The translation vector this component should be initialized with.
         */
        explicit TranslationStateComponent(const helios::math::vec3f translation) :
            translation_(translation) {}

        /**
         * @brief Resets the translation vector to 0-vector when acquired.
         */
        void onAcquire() noexcept{
            translation_ = helios::math::vec3f{};
        }


        /**
         * @brief Resets the translation vector to 0-vector when released.
         */
        void onRelease() noexcept {
            translation_ = helios::math::vec3f{};
        }

        /**
         * @brief Sets the absolute translation.
         *
         * @param translation The new position vector.
         */
        void setTranslation (helios::math::vec3f translation) {
            translation_ = translation;
        }

        /**
         * @brief Translates the current position by a delta vector.
         *
         * @param translation The vector to add to the current position.
         */
        void translateBy(helios::math::vec3f translation) {
            translation_ = translation_ + translation;
        }

        /**
         * @brief Returns the current translation vector.
         *
         * @return The current position.
         */
        [[nodiscard]] helios::math::vec3f translation() const noexcept {
            return translation_;
        }
    };

};