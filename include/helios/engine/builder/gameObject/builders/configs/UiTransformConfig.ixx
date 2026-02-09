/**
 * @file UiTransformConfig.ixx
 * @brief Configuration for spatial transform components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.UiTransformConfig;

import helios.engine.ecs.GameObject;

import helios.engine.modules.ui;

import helios.math;

import helios.core;
import helios.engine.core;

import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for spatial transform setup.
     *
     * Automatically adds ComposeTransformComponent and provides
     * methods for setting scale and translation.
     */
    class UiTransformConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
         helios::engine::ecs::GameObject gameObject_;


    public:

        /**
         * @brief Constructs a UiTransformConfig and adds ComposeTransformComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit UiTransformConfig(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {
            gameObject_.getOrAdd<helios::engine::modules::spatial::transform::components::ComposeTransformComponent>();
            gameObject_.getOrAdd<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();
            gameObject_.add<helios::engine::modules::ui::transform::components::UiTransformComponent>();
        }

        /**
         * @brief Sets the anchor point for UI positioning.
         *
         * @param anchor The anchor position (e.g., TopLeft, Center).
         *
         * @return Reference to this config for chaining.
         */
        UiTransformConfig& anchor(const helios::engine::modules::ui::layout::Anchor anchor) {
            gameObject_.get<helios::engine::modules::ui::transform::components::UiTransformComponent>()
                        ->setAnchor(anchor);
            return *this;
        }

        /**
         * @brief Sets the viewport for this UI element.
         *
         * @param viewportId The ID of the viewport to attach to.
         *
         * @return Reference to this config for chaining.
         */
        UiTransformConfig& viewport(const helios::engine::core::data::ViewportId viewportId) {
            gameObject_.get<helios::engine::modules::ui::transform::components::UiTransformComponent>()
                        ->setViewportId(viewportId);
            return *this;
        }

        /**
         * @brief Sets the pivot point for transformations.
         *
         * @param anchor The pivot position.
         *
         * @return Reference to this config for chaining.
         */
        UiTransformConfig& pivot(const helios::engine::modules::ui::layout::Anchor anchor) {
            gameObject_.get<helios::engine::modules::ui::transform::components::UiTransformComponent>()
                        ->setPivot(anchor);
            return *this;
        }

        /**
         * @brief Sets the margins for UI positioning.
         *
         * @param margins The margins as vec4f (left, top, right, bottom).
         *
         * @return Reference to this config for chaining.
         */
        UiTransformConfig& margins(const helios::math::vec4f margins) {
            gameObject_.get<helios::engine::modules::ui::transform::components::UiTransformComponent>()
                        ->setMargins(margins);
            return *this;
        }



    };

}

