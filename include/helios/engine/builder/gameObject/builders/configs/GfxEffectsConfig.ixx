/**
 * @file GfxEffectsConfig.ixx
 * @brief Configuration for visual effects components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.GfxEffectsConfig;

import helios.engine.ecs.GameObject;

import helios.engine.modules.effects.gfx.components.SpinComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;

import helios.math;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for visual effects.
     *
     * Provides methods for adding continuous rotation (spin)
     * and other visual effects to a GameObject.
     */
    class GfxEffectsConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a GfxEffectsConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit GfxEffectsConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Adds continuous rotation (spin) effect.
         *
         * @param degreesPerSecond Rotation speed in degrees per second.
         * @param spinAxis The axis of rotation (normalized).
         *
         * @return Reference to this config for chaining.
         */
        GfxEffectsConfig& spin(const float degreesPerSecond, const helios::math::vec3f spinAxis) {
            gameObject_->add<helios::engine::modules::effects::gfx::components::SpinComponent>(
                degreesPerSecond, spinAxis
            );
            gameObject_->getOrAdd<helios::engine::modules::spatial::transform::components::RotationStateComponent>();
            return *this;
        }


    };

}

