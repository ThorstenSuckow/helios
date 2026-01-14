/**
 * @file TranslationStateComponent.ixx
 * @brief Component for storing the current translation (position) state of an entity.
 */
module;

#include <cmath>

export module helios.engine.modules.spatial.transform.components.TranslationStateComponent;

import helios.engine.ecs.CloneableComponent;

import helios.math;

export namespace helios::engine::modules::spatial::transform::components {

    /**
     * @brief Component that holds the current translation (position) state.
     *
     * @details
     * This component stores the accumulated translation vector. It acts as a data
     * container for the ComposeTransformSystem to build the final translation matrix.
     * It separates the raw position data from the computed TransformComponent.
     */
    class TranslationStateComponent : public helios::engine::ecs::CloneableComponent<TranslationStateComponent> {

        /**
         * @brief The current translation vector.
         */
        helios::math::vec3f translation_{};

    public:

        /**
         * @brief Default constructor.
         */
        TranslationStateComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        explicit TranslationStateComponent(const TranslationStateComponent& other) :
            translation_(other.translation_){
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