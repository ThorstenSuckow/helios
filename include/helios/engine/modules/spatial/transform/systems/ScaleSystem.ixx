/**
 * @file ScaleSystem.ixx
 * @brief System for applying scale transformations based on ScaleStateComponent.
 */
module;



export module helios.engine.modules.spatial.transform.systems.ScaleSystem;

import helios.engine.ecs.System;
import helios.math;

import helios.core.units.Unit;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.modules.spatial.transform.components.ScaleStateComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

import helios.engine.mechanics.lifecycle.components.Active;

export namespace helios::engine::modules::spatial::transform::systems {

    /**
     * @brief System that applies scaling to entities based on their ScaleStateComponent.
     *
     * @details
     * This system converts the desired world-space dimensions from ScaleStateComponent
     * into scale factors relative to the model's original AABB. It handles unit
     * conversion (e.g., centimeters to meters) based on the ScaleStateComponent's unit.
     *
     * Required components:
     * - ScaleStateComponent (desired dimensions and unit)
     * - ModelAabbComponent (original model bounds)
     * - ComposeTransformComponent (receives scale updates)
     */
    class ScaleSystem : public helios::engine::ecs::System {

    public:

        /**
         * @brief Called when the system is added to a GameWorld.
         *
         * @param gameWorld Pointer to the GameWorld this system belongs to.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) noexcept override {
            System::init(gameWorld);
        }

        /**
         * @brief Updates scale for all entities with dirty ScaleComponents.
         *
         * @details For each entity with the required components, calculates the scale factor
         * needed to resize the model from its original dimensions to the desired
         * world-space dimensions.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, mab, sc, tc, active] : gameWorld_->view<
                helios::engine::modules::rendering::model::components::ModelAabbComponent,
                helios::engine::modules::spatial::transform::components::ScaleStateComponent,
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                if (!sc->isDirty()) {
                    continue;
                }

                // Get current model size and desired size
                auto cscale = mab->aabb().size();
                auto wscale = sc->scaling();

                auto currentScale = tc->localScaling();

                auto unit = sc->unit();

                // Calculate scale factors: desired_size / current_size
                // Convert desired size to engine units (meters)
                auto scale = helios::math::vec3f{
                    wscale[0] != 0 && cscale[0] != 0 ? helios::core::units::from(wscale[0], unit) / cscale[0] : currentScale[0],
                    wscale[1] != 0 && cscale[1] != 0 ? helios::core::units::from(wscale[1], unit) / cscale[1] : currentScale[1],
                    wscale[2] != 0 && cscale[2] != 0 ? helios::core::units::from(wscale[2], unit) / cscale[2] : currentScale[2]
                };

                tc->setLocalScale(scale);
            }
        }

    };

}
