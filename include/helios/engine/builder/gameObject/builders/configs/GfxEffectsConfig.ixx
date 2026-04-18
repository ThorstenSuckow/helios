/**
 * @file GfxEffectsConfig.ixx
 * @brief Configuration for visual effects components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.GfxEffectsConfig;


import helios.gameplay.effects.gfx.components.SpinComponent;
import helios.spatial.transform.components.RotationStateComponent;

import helios.math;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for visual effects.
     *
     * Provides methods for adding continuous rotation (spin)
     * and other visual effects to a GameObject.
     */

    template<typename Entity>
    class GfxEffectsConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a GfxEffectsConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit GfxEffectsConfig(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Adds continuous rotation (spin) effect.
         *
         * @param degreesPerSecond Rotation speed in degrees per second.
         * @param spinAxis The axis of rotation (normalized).
         *
         * @return Reference to this config for chaining.
         */
        GfxEffectsConfig& spin(const float degreesPerSecond, const helios::math::vec3f spinAxis) {
            gameObject_.template add<helios::gameplay::effects::gfx::components::SpinComponent<Handle_type>>(
                degreesPerSecond, spinAxis
            );
            gameObject_.template getOrAdd<helios::spatial::transform::components::RotationStateComponent<Handle_type>>();
            return *this;
        }


    };

}
