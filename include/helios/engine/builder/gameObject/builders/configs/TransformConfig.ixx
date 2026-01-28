/**
 * @file TransformConfig.ixx
 * @brief Configuration for spatial transform components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.TransformConfig;

import helios.engine.ecs.GameObject;

import helios.engine.modules.spatial.transform.components;

import helios.math;
import helios.engine.core.units;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for spatial transform setup.
     *
     * Automatically adds ComposeTransformComponent and provides
     * methods for setting scale and translation.
     */
    class TransformConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
         helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a TransformConfig and adds ComposeTransformComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit TransformConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {
            gameObject_->add<helios::engine::modules::spatial::transform::components::ComposeTransformComponent>();

        }

        /**
         * @brief Sets the scale of the entity.
         *
         * @param scale Scale factors for x, y, z axes.
         * @param unit The unit of measurement (default: Meter).
         *
         * @return Reference to this config for chaining.
         */
        TransformConfig& scale(
            const helios::math::vec3f scale,
            const helios::core::units::Unit unit = helios::core::units::Unit::Meter
        ) {
            gameObject_->add<helios::engine::modules::spatial::transform::components::ScaleStateComponent>(scale, unit);
            return *this;
        }

        /**
         * @brief Sets the translation (position) of the entity.
         *
         * @param translation Position in world space.
         *
         * @return Reference to this config for chaining.
         */
        TransformConfig& translate(const helios::math::vec3f translation) {
            gameObject_->getOrAdd<helios::engine::modules::spatial::transform::components::TranslationStateComponent>()
                        .setTranslation(translation);
            return *this;
        }

    };

}

