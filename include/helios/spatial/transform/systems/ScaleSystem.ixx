/**
 * @file ScaleSystem.ixx
 * @brief System for applying scale transformations based on ScaleStateComponent.
 */
module;



export module helios.spatial.transform.systems.ScaleSystem;


import helios.math;

import helios.core.units.Unit;

import helios.runtime.messaging.command.NullCommandBuffer;

import helios.runtime.world.GameObject;
import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;

import helios.spatial.transform.components.ScaleStateComponent;
import helios.spatial.transform.components.ComposeTransformComponent;

import helios.rendering.model.components.ModelAabbComponent;

import helios.ecs.components.Active;

import helios.runtime.messaging.command.concepts.IsCommandBufferLike;
import helios.ecs.concepts.IsEntityHandle;
import helios.runtime.world.tags.SystemRole;

using namespace helios::runtime::messaging::command;
using namespace helios::runtime::concepts;
using namespace helios::ecs::concepts;
export namespace helios::spatial::transform::systems {

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
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer> && IsEntityHandle<THandle>
    class ScaleSystem {


    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;
        /**
         * @brief Updates scale for all entities with dirty ScaleComponents.
         *
         * @details For each entity with the required components, calculates the scale factor
         * needed to resize the model from its original dimensions to the desired
         * world-space dimensions.
         *
         * @param updateContext Context containing deltaTime and other frame data.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, mab, sc, tc, active] : updateContext.view<
                THandle,
                helios::rendering::model::components::ModelAabbComponent<THandle>,
                helios::spatial::transform::components::ScaleStateComponent<THandle>,
                helios::spatial::transform::components::ComposeTransformComponent<THandle>,
                helios::ecs::components::Active<THandle>
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
