/**
 * @file TransformConfig.ixx
 * @brief Configuration for spatial transform components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.TransformConfig;


import helios.spatial.transform.components;

import helios.math;
import helios.core.units;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for spatial transform setup.
     *
     * Automatically adds ComposeTransformComponent and provides
     * methods for setting scale and translation.
     */
    template<typename Entity>
    class TransformConfig {

        using Handle_type = typename Entity::Handle_type;

         Entity gameObject_;

    public:

        /**
         * @brief Constructs a TransformConfig and adds ComposeTransformComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit TransformConfig(Entity gameObject) : gameObject_(gameObject) {
            gameObject_.template add<helios::spatial::transform::components::ComposeTransformComponent<Handle_type>>();

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
            gameObject_.template add<helios::spatial::transform::components::ScaleStateComponent<Handle_type>>(scale, unit);
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
            gameObject_.template getOrAdd<helios::spatial::transform::components::TranslationStateComponent<Handle_type>>()
                        .setTranslation(translation);
            return *this;
        }

        /**
         * @brief Sets the initial heading rotation of the entity.
         *
         * @param degrees Rotation angle in degrees.
         * @param axis Rotation axis.
         *
         * @return Reference to this config for chaining.
         */
        TransformConfig& rotate(const float degrees, const helios::math::vec3f axis) {
            auto& rsc = gameObject_.template getOrAdd<helios::spatial::transform::components::RotationStateComponent<Handle_type>>();

            rsc.setHeadingRotationAngle(degrees);
            rsc.setHeadingRotationAxis(axis);

            return *this;
        }

    };

}

