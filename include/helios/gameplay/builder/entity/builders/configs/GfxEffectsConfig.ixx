/**
 * @file GfxEffectsConfig.ixx
 * @brief Configuration for visual effects components.
 */
module;

export module helios.gameplay.builder.entity.builders.configs.GfxEffectsConfig;


import helios.gameplay.effects.gfx.components.SpinComponent;
import helios.spatial.transform.components.RotationStateComponent;

import helios.math;

export namespace helios::gameplay::builder::entity::builders::configs {

    /**
     * @brief Fluent configuration for visual effects.
     *
     * Provides methods for adding continuous rotation (spin)
     * and other visual effects to a Entity.
     */

    template<typename Entity>
    class GfxEffectsConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a GfxEffectsConfig.
         *
         * @param entity Target Entity to configure.
         */
        explicit GfxEffectsConfig(Entity entity) : entity_(entity) {}

        /**
         * @brief Adds continuous rotation (spin) effect.
         *
         * @param degreesPerSecond Rotation speed in degrees per second.
         * @param spinAxis The axis of rotation (normalized).
         *
         * @return Reference to this config for chaining.
         */
        GfxEffectsConfig& spin(const float degreesPerSecond, const helios::math::vec3f spinAxis) {
            entity_.template add<helios::gameplay::effects::gfx::components::SpinComponent<Handle_type>>(
                degreesPerSecond, spinAxis
            );
            entity_.template getOrAdd<helios::spatial::transform::components::RotationStateComponent<Handle_type>>();
            return *this;
        }


    };

}
