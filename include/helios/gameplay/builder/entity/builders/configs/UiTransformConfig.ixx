/**
 * @file UiTransformConfig.ixx
 * @brief Configuration for spatial transform components.
 */
module;

export module helios.gameplay.builder.entity.builders.configs.UiTransformConfig;

import helios.gameplay.common.types;

import helios.ui;

import helios.math;

import helios.core;
import helios.core;

import helios.rendering.viewport;

import helios.spatial.transform.components.TranslationStateComponent;
import helios.spatial.transform.components.ComposeTransformComponent;

export namespace helios::gameplay::builder::entity::builders::configs {

    /**
     * @brief Fluent configuration for spatial transform setup.
     *
     * Automatically adds ComposeTransformComponent and provides
     * methods for setting scale and translation.
     */
    template<typename Entity>
    class UiTransformConfig {

        using Handle_type = typename Entity::Handle_type;

         Entity entity_;


    public:

        /**
         * @brief Constructs a UiTransformConfig and adds ComposeTransformComponent.
         *
         * @param entity Target Entity to configure.
         */
        explicit UiTransformConfig(Entity entity) : entity_(entity) {
            entity_.template getOrAdd<helios::spatial::transform::components::ComposeTransformComponent<Handle_type>>();
            entity_.template getOrAdd<helios::spatial::transform::components::TranslationStateComponent<Handle_type>>();
            entity_.template add<helios::ui::transform::components::UiTransformComponent<Handle_type>>();
        }

        /**
         * @brief Sets the anchor point for UI positioning.
         *
         * @param anchor The anchor position (e.g., TopLeft, Center).
         *
         * @return Reference to this config for chaining.
         */
        UiTransformConfig& anchor(const helios::ui::layout::Anchor anchor) {
            entity_.template get<helios::ui::transform::components::UiTransformComponent<Handle_type>>()
                        ->setAnchor(anchor);
            return *this;
        }

        /**
         * @brief Sets the viewport for this UI element.
         *
         * @param viewportHandle The Handle of the viewport to attach to.
         *
         * @return Reference to this config for chaining.
         */
        UiTransformConfig& viewport(const helios::rendering::viewport::types::ViewportHandle viewportHandle) {
            entity_.template get<helios::ui::transform::components::UiTransformComponent<Handle_type>>()
                        ->setViewportHandle(viewportHandle);
            return *this;
        }

        /**
         * @brief Sets the pivot point for transformations.
         *
         * @param anchor The pivot position.
         *
         * @return Reference to this config for chaining.
         */
        UiTransformConfig& pivot(const helios::ui::layout::Anchor anchor) {
            entity_.template get<helios::ui::transform::components::UiTransformComponent<Handle_type>>()
                        ->setPivot(anchor);
            return *this;
        }

        /**
         * @brief Sets the offsets for UI positioning.
         *
         * @param offsets The offsets as vec4f (top, right, bottom, left).
         *
         * @return Reference to this config for chaining.
         */
        UiTransformConfig& offsets(const helios::math::vec4f offsets) {
            entity_.template get<helios::ui::transform::components::UiTransformComponent<Handle_type>>()
                        ->setOffsets(offsets);
            return *this;
        }



    };

}

